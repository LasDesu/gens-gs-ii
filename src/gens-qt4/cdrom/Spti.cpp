/***************************************************************************
 * gens-qt4: Gens Qt4 UI.                                                  *
 * Spti.hpp: Win32 SPTI wrapper.                                           *
 *                                                                         *
 * Copyright (c) 2011 by David Korth.                                      *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify it *
 * under the terms of the GNU General Public License as published by the   *
 * Free Software Foundation; either version 2 of the License, or (at your  *
 * option) any later version.                                              *
 *                                                                         *
 * This program is distributed in the hope that it will be useful, but     *
 * WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with this program; if not, write to the Free Software Foundation, Inc., *
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.           *
 ***************************************************************************/

// TODO: Move this to libgens later.

#include "Spti.hpp"

// C includes.
#include <ctype.h>
#include <stdio.h>

// C++ includes.
#include <string>
using std::string;

// SPTI/SCSI headers.
#include <devioctl.h>
#include "gens_scsi_win32.h"

namespace GensQt4
{

Spti::Spti(char drive_letter)
{
	drive_letter = toupper(drive_letter);
	
	if (drive_letter < 'A' || drive_letter > 'Z')
	{
		// Invalid drive letter specified.
		m_hDevice = INVALID_HANDLE_VALUE;
		return;
	}
	
	// Create the pathname.
	char spti_pathname[7] = {'\\', '\\', '.', '\\', drive_letter, ':', 0};
	
	// Open the specified drive using SPTI.
	m_hDevice = CreateFileA(spti_pathname,				// lpFileName
				GENERIC_READ | GENERIC_WRITE,		// dwDesiredAccess
				FILE_SHARE_READ | FILE_SHARE_WRITE,	// dwShareMode
				NULL,					// lpSecurityAttributes
				OPEN_EXISTING,				// dwCreationDisposition,
				FILE_ATTRIBUTE_NORMAL,			// dwFlagsAndAttributes
				NULL);					// hTemplateFile
}

Spti::~Spti()
{
	close();
}


/**
 * close(): Close the device file handle.
 */
void Spti::close(void)
{
	if (m_hDevice != INVALID_HANDLE_VALUE)
		CloseHandle(m_hDevice);
	m_hDevice = INVALID_HANDLE_VALUE;
}


/**
 * TrimEndSpaces(): Trim spaces from the end of a text buffer.
 * @param buf Text buffer.
 * @param len Length of the text buffer.
 */
void Spti::TrimEndSpaces(char *buf, int len)
{
	for (len--; len >= 0; len--)
	{
		if (isspace(buf[len]))
			buf[len] = 0x00;
		else
			break;
	}
}


/**
 * ScsiSendCdb(): Send a SCSI command buffer to m_hDevice.
 * @return 0 on error; non-zero on success. (Win32-style)
 */
int Spti::scsiSendCdb(void *cdb, unsigned char cdb_length,
			void *buffer, unsigned int buffer_length,
			int data_in)
{
	// Based on http://www.codeproject.com/KB/system/mydvdregion.aspx
	DWORD returned;
	int ret;
	
	// Size of SCSI_PASS_THROUGH + 96 bytes for sense data.
	uint8_t cmd[sizeof(SCSI_PASS_THROUGH_DIRECT) + 96];
	memset(cmd, 0x00, sizeof(cmd));
	
	// Shortcut to the buffer.
	SCSI_PASS_THROUGH_DIRECT *pcmd = (SCSI_PASS_THROUGH_DIRECT*)cmd;
	
	// Copy the CDB to the SCSI_PASS_THROUGH structure.
	memcpy(pcmd->Cdb, cdb, cdb_length);
	
	// Initialize the other SCSI command variables.
	pcmd->DataBuffer = buffer;
	pcmd->DataTransferLength = buffer_length;
	pcmd->DataIn = data_in;
	pcmd->CdbLength = cdb_length;
	pcmd->Length = sizeof(SCSI_PASS_THROUGH_DIRECT);
	pcmd->SenseInfoLength = (sizeof(cmd) - sizeof(SCSI_PASS_THROUGH_DIRECT));
	pcmd->SenseInfoOffset = sizeof(SCSI_PASS_THROUGH_DIRECT);
	pcmd->TimeOutValue = 6000; // TODO: 6000?
	
	ret = DeviceIoControl(m_hDevice, IOCTL_SCSI_PASS_THROUGH_DIRECT,
				(LPVOID)&cmd, sizeof(cmd),
				(LPVOID)&cmd, sizeof(cmd),
				&returned, NULL);
	
	// TODO: Better error handling.
	DWORD err = GetLastError();
	if (err != ERROR_SUCCESS)
		fprintf(stderr, "%s(): Error: 0x%08X\n", __func__, (unsigned int)err);
	return ret;
}


/** SCSI functions. **/


/**
 * scsiInquiry(): SCSI_INQUIRY command.
 * Inquiry data is stored locally and is available on request.
 * @return 0 on success; non-zero on error.
 */
int Spti::scsiInquiry(void)
{
	CDB_SCSI inquiry;
	SCSI_INQUIRY_STD_DATA data;
	
	memset(&inquiry, 0x00, sizeof(inquiry));
	memset(&data, 0x00, sizeof(data));
	
	// Set SCSI operation type.
	inquiry.OperationCode = SCSI_INQUIRY;
	inquiry.AllocationLength = sizeof(data);
	
	// Send the SCSI CDB.
	if (!scsiSendCdb(&inquiry, sizeof(inquiry), &data, sizeof(data)))
		return -1;
	
	// Drive inquiry successful.
	// Get the information.
	
	// Device type.
	m_inq_dev_type = data.peripheral_device_type;
	
	// Drive vendor.
	char buf[12];
	memcpy(buf, data.vendor_id, sizeof(data.vendor_id));
	buf[sizeof(data.vendor_id)] = 0x00;
	TrimEndSpaces(buf, sizeof(data.vendor_id));
	m_inq_vendor = string(buf);
	
	// Drive model.
	memcpy(buf, data.product_id, sizeof(data.product_id));
	buf[sizeof(data.product_id)] = 0x00;
	TrimEndSpaces(buf, sizeof(data.product_id));
	m_inq_model = string(buf);
	
	// Firmware revision.
	memcpy(buf, data.product_revision_level, sizeof(data.product_revision_level));
	buf[sizeof(data.product_revision_level)] = 0x00;
	TrimEndSpaces(buf, sizeof(data.product_revision_level));
	m_inq_firmware = string(buf);
	
	// SCSI_INQUIRY completed successfully.
	return 0;
}


/**
 * isMediumPresent(): Check if a disc is present.
 * @return True if a disc is present; false if not.
 */
bool Spti::isMediumPresent(void)
{
	// TODO: This doesn't seem to work if e.g. START/STOP wasn't requested first.
	CDB_SCSI tst_u_rdy;
	memset(&tst_u_rdy, 0x00, sizeof(tst_u_rdy));
	tst_u_rdy.OperationCode = SCSI_TST_U_RDY;
	
	if (!scsiSendCdb(&tst_u_rdy, sizeof(tst_u_rdy), NULL, 0))
		return false;
	
	// Request sense data.
	CDB_SCSI req_sense;
	SCSI_DATA_REQ_SENSE data_req_sense;
	
	memset(&req_sense, 0x00, sizeof(req_sense));
	memset(&data_req_sense, 0x00, sizeof(data_req_sense));
	
	req_sense.OperationCode = SCSI_REQ_SENSE;
	req_sense.AllocationLength = sizeof(data_req_sense);
	
	if (scsiSendCdb(&req_sense, sizeof(req_sense), &data_req_sense, sizeof(data_req_sense)))
	{
		// REQ_SENSE successful.
		return (data_req_sense.SenseKey == 0);
	}
	
	// REQ_SENSE was not successful.
	return false;
}

}
