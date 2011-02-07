/***************************************************************************
 * gens-qt4: Gens Qt4 UI.                                                  *
 * GeneralConfigWindow.hpp: General Configuration Window.                  *
 *                                                                         *
 * Copyright (c) 1999-2002 by Stéphane Dallongeville.                      *
 * Copyright (c) 2003-2004 by Stéphane Akhoun.                             *
 * Copyright (c) 2008-2011 by David Korth.                                 *
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

#ifndef __GENS_QT4_GENERALCONFIGWINDOW_HPP__
#define __GENS_QT4_GENERALCONFIGWINDOW_HPP__

#include "ui_GeneralConfigWindow.h"

// Qt includes.
#include <QtGui/QMainWindow>
#include <QtGui/QColor>

#ifdef Q_WS_MAC
// Mac OS X:
// Use a unified toolbar instead of tabs.
class QStackedWidget;
class QToolBar;
class QAction;
#endif

// GensLineEdit widget.
#include "widgets/GensLineEdit.hpp"

// C includes.
#include <stdio.h>
#include <math.h>

// libgens: Sega CD Boot ROM database.
#include "libgens/Data/mcd_rom_db.h"

namespace GensQt4
{

class GeneralConfigWindow : public QMainWindow, public Ui::GeneralConfigWindow
{
	Q_OBJECT
	
	public:
		static void ShowSingle(QWidget *parent = NULL);
	
	protected:
		GeneralConfigWindow(QWidget *parent = NULL);
		virtual ~GeneralConfigWindow();
		
		// Button CSS colors.
		static const QString ms_sCssBtnColors;
		
		// Warning string.
		static const QString ms_sWarning;
		
		// Qt window flags.
		static const Qt::WindowFlags ms_WindowFlags;
		
		void setApplyButtonEnabled(bool enabled)
		{
			QPushButton *btnApply = buttonBox->button(QDialogButtonBox::Apply);
			if (btnApply)
				btnApply->setEnabled(enabled);
		}
		
		/**
		 * QColor_Grayscale(): Convert a QColor to grayscale.
		 * This uses the standard ITU-R BT.601 grayscale conversion matrix.
		 * @return Grayscale component, or -1 if color is invalid.
		 */
		inline int QColor_Grayscale(const QColor& color)
		{
			if (!color.isValid())
				return -1;
			
			double grayD = ((double)color.red() * 0.299) +
				       ((double)color.green() * 0.587) +
				       ((double)color.blue() * 0.114);
			int grayI = rint(grayD);
			
			// Clamp the grayscale value at [0, 255].
			if (grayI < 0)
				grayI = 0;
			if (grayI > 255)
				grayI = 255;
			return grayI;
		}
		
#ifdef Q_WS_MAC
		// Mac OS X:
		// Use a unified toolbar instead of tabs.
		QStackedWidget *stackedWidget;
		QToolBar *toolBar;
		void setupUi_mac(void);
#endif
	
	protected slots:
		void accept(void) { apply(); this->close(); }
		void apply(void);
		void reject(void) { this->close(); }
		
		void reload(void);
	
		// Mac OS X: Toolbar action group.
		// moc doesn't properly support OS-specific slots,
		// so we need to define this slot on all systems.
		void toolbarTriggered(QAction *action);
	
	private:
		static GeneralConfigWindow *m_GeneralConfigWindow;
		
		/** Onscreen Display **/
		QColor osdSelectColor(const QString& color_id, const QColor& init_color);
		
		// Onscreen Display: Colors.
		QColor m_osdFpsColor;
		QColor m_osdMsgColor;
		
		/** Sega CD: Boot ROM **/
		
		// Sega CD: Boot ROM filesize.
		static const int MCD_ROM_FILESIZE = 131072;
		
		// Sega CD: Select Boot ROM file.
		void mcdSelectRomFile(const QString& rom_id, GensLineEdit *txtRomFile);
		
		// Sega CD: Update Boot ROM file status.
		QString mcdUpdateRomFileStatus(GensLineEdit *txtRomFile, int region_code);
		
		// Sega CD: Display Boot ROM file status.
		void mcdDisplayRomFileStatus(const QString& rom_id, const QString& rom_desc);
		
		// Sega CD: Boot ROM file information.
		QString sMcdRomStatus_USA;
		QString sMcdRomStatus_EUR;
		QString sMcdRomStatus_JPN;
		
		/** External Programs **/
		QString sExtPrgStatus_UnRAR;
		void extprgDisplayFileStatus(const QString& file_id, const QString& file_desc);
	
	private slots:
		/**
		 * settingChanged(): A setting has changed.
		 * The "Apply" button should be enabled.
		 * TODO: On Mac OS X, apply the change immediately.
		 */
		void settingChanged(void)
			{ setApplyButtonEnabled(true); }
		
		/** Onscreen Display **/
		void on_btnOsdFpsColor_clicked(void);
		void on_btnOsdMsgColor_clicked(void);
		
		/** Sega CD: Boot ROM **/
		
		void on_btnMcdRomUSA_clicked(void);
		void on_btnMcdRomEUR_clicked(void);
		void on_btnMcdRomJPN_clicked(void);
		
		void on_txtMcdRomUSA_focusIn(void);
		void on_txtMcdRomEUR_focusIn(void);
		void on_txtMcdRomJPN_focusIn(void);
		
		void on_txtMcdRomUSA_textChanged(void);
		void on_txtMcdRomEUR_textChanged(void);
		void on_txtMcdRomJPN_textChanged(void);
		
		/** External Programs **/
		
		void on_btnExtPrgUnRAR_clicked(void);
		void on_txtExtPrgUnRAR_focusIn(void);
		void on_txtExtPrgUnRAR_textChanged(void);
};

}

#endif /* __GENS_QT4_GENERALCONFIGWINDOW_HPP__ */
