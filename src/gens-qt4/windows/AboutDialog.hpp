/***************************************************************************
 * gens-qt4: Gens Qt4 UI.                                                  *
 * AboutDialog.hpp: About Dialog.                                          *
 *                                                                         *
 * Copyright (c) 1999-2002 by Stéphane Dallongeville.                      *
 * Copyright (c) 2003-2004 by Stéphane Akhoun.                             *
 * Copyright (c) 2008-2014 by David Korth.                                 *
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

#ifndef __GENS_QT4_ABOUTWINDOW_HPP__
#define __GENS_QT4_ABOUTWINDOW_HPP__

#include <QtGui/QDialog>
#include "ui_AboutDialog.h"

namespace GensQt4
{

class AboutDialog : public QDialog, public Ui::AboutDialog
{
	Q_OBJECT
	
	public:
		static void ShowSingle(QWidget *parent = nullptr);
	
	protected:
		AboutDialog(QWidget *parent = nullptr);
		virtual ~AboutDialog();
		
		// State change event. (Used for switching the UI language at runtime.)
		void changeEvent(QEvent *event);
		
		// Initialize the About Dialog text.
		void initAboutDialogText(void);
		bool m_scrlAreaInit;
		
		// Included libraries.
		static QString GetIncLibraries(void);
		
		// Debug information.
		static QString GetDebugInfo(void);
#ifdef Q_OS_WIN32
		static QString GetCodePageInfo(void);
#endif /* Q_OS_WIN32 */
	
	private:
		static AboutDialog *m_AboutDialog;
};

}

#endif /* __GENS_QT4_ABOUTWINDOW_HPP__ */
