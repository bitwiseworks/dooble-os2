/*
** Copyright (c) 2008 - present, Alexis Megas.
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 3. The name of the author may not be used to endorse or promote products
**    derived from Dooble without specific prior written permission.
**
** DOOBLE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
** IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
** IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
** NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
** DOOBLE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "dooble.h"
#include "dooble_accepted_or_blocked_domains.h"
#include "dooble_application.h"
#include "dooble_clear_items.h"
#include "dooble_cookies_window.h"
#include "dooble_cryptography.h"
#include "dooble_history_window.h"
#include "dooble_popup_menu.h"

dooble_popup_menu::dooble_popup_menu(qreal zoom_factor, QWidget *parent):
  QDialog(parent)
{
  m_accept_on_click = true;
  m_ui.setupUi(this);

  if(dooble::s_cryptography && dooble::s_cryptography->authenticated())
    m_ui.authenticate->setEnabled(false);
  else
    m_ui.authenticate->setEnabled(dooble_settings::has_dooble_credentials());

  if(!dooble_settings::has_dooble_credentials())
    m_ui.authenticate->setToolTip
      (tr("Permanent credentials have not been prepared."));

  m_ui.zoom_frame->setVisible
    (dooble_settings::
     zoom_frame_location_string(dooble_settings::
				setting("zoom_frame_location_index").
				toInt()) == "popup_menu");
  connect(m_ui.authenticate,
	  SIGNAL(clicked(void)),
	  this,
	  SIGNAL(authenticate(void)));

  foreach(auto tool_button, findChildren<QToolButton *> ())
    {
      connect(tool_button,
	      SIGNAL(clicked(void)),
	      this,
	      SLOT(slot_tool_button_clicked(void)));

      if(m_ui.exit_dooble == tool_button)
#ifdef Q_OS_MACOS
	tool_button->setStyleSheet
	  ("QToolButton {border: none;}"
	   "QToolButton::hover {background-color: #b71c1c;}");
#else
        tool_button->setStyleSheet
	  ("QToolButton::hover {background-color: #b71c1c;}");
#endif
      else
#ifdef Q_OS_MACOS
	tool_button->setStyleSheet
	  ("QToolButton {border: none;}"
	   "QToolButton::hover {background-color: darkorange;}");
#else
        {
	}
#endif
    }

  connect(dooble::s_application,
	  SIGNAL(dooble_credentials_authenticated(bool)),
	  this,
	  SLOT(slot_authenticated(bool)));
  connect(dooble::s_settings,
	  SIGNAL(dooble_credentials_authenticated(bool)),
	  this,
	  SLOT(slot_settings_applied(void)));
  connect(dooble::s_settings,
	  SIGNAL(dooble_credentials_created(void)),
	  this,
	  SLOT(slot_settings_applied(void)));
  prepare_icons();
  setWindowFlag(Qt::WindowStaysOnTopHint, true);
  slot_zoomed(zoom_factor);
}

dooble *dooble_popup_menu::find_parent_dooble(void) const
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

  auto parent = parentWidget();

  do
    {
      if(qobject_cast<dooble *> (parent))
	{
	  QApplication::restoreOverrideCursor();
	  return qobject_cast<dooble *> (parent);
	}
      else if(parent)
	parent = parent->parentWidget();
    }
  while(parent);

  QApplication::restoreOverrideCursor();
  return nullptr;
}

void dooble_popup_menu::hide_for_non_web_page(bool state)
{
  m_ui.print->setEnabled(!state);
  m_ui.save_page->setEnabled(!state);
}

void dooble_popup_menu::prepare_icons(void)
{
  auto icon_set(dooble_settings::setting("icon_set").toString());
  auto use_material_icons(dooble_settings::use_material_icons());

  m_ui.authenticate->setIcon
    (QIcon::fromTheme(use_material_icons + "dialog-password",
		      QIcon(QString(":/%1/48/authenticate.png").
			    arg(icon_set))));
  m_ui.blocked_domains->setIcon
    (QIcon::fromTheme(use_material_icons + "process-stop",
		      QIcon(QString(":/%1/48/blocked_domains.png").
			    arg(icon_set))));
  m_ui.clear_items->setIcon
    (QIcon::fromTheme(use_material_icons + "user-trash",
		      QIcon(QString(":/%1/48/clear_items.png").arg(icon_set))));
  m_ui.cookies->setIcon
    (QIcon::fromTheme(use_material_icons + "preferences-web-browser-cookies",
		      QIcon(QString(":/%1/48/cookies.png").arg(icon_set))));
  m_ui.exit_dooble->setIcon
    (QIcon::fromTheme(use_material_icons + "application-exit",
		      QIcon(QString(":/%1/48/exit_dooble.png").arg(icon_set))));
  m_ui.history->setIcon
    (QIcon::fromTheme(use_material_icons + "deep-history",
		      QIcon(QString(":/%1/48/history.png").arg(icon_set))));
  m_ui.new_private_window->setIcon
    (QIcon::fromTheme(use_material_icons + "view-private",
		      QIcon(QString(":/%1/48/new_private_window.png").
			    arg(icon_set))));
  m_ui.new_tab->setIcon
    (QIcon::fromTheme(use_material_icons + "folder-new",
		      QIcon(QString(":/%1/48/new_tab.png").arg(icon_set))));
  m_ui.new_window->setIcon
    (QIcon::fromTheme(use_material_icons + "window-new",
		      QIcon(QString(":/%1/48/new_window.png").arg(icon_set))));
  m_ui.print->setIcon
    (QIcon::fromTheme(use_material_icons + "document-print",
		      QIcon(QString(":/%1/48/print.png").arg(icon_set))));
  m_ui.save_page->setIcon
    (QIcon::fromTheme(use_material_icons + "document-save",
		      QIcon(QString(":/%1/48/save.png").arg(icon_set))));
  m_ui.settings->setIcon
    (QIcon::fromTheme(use_material_icons + "preferences-system",
		      QIcon(QString(":/%1/48/settings.png").arg(icon_set))));

  int preferred_height = 50;
  int preferred_width = 50;

  foreach(auto tool_button, findChildren<QToolButton *> ())
    {
      tool_button->setIconSize(QSize(48, 48));
      preferred_height = qMax
	(preferred_height, tool_button->sizeHint().height());
      preferred_width = qMax
	(preferred_width, tool_button->sizeHint().width());
    }

  foreach(auto tool_button, findChildren<QToolButton *> ())
    if(m_ui.zoom_in != tool_button &&
       m_ui.zoom_out != tool_button &&
       m_ui.zoom_reset != tool_button)
      {
	tool_button->setMaximumSize(QSize(preferred_width, preferred_height));
	tool_button->setMinimumSize(QSize(preferred_width, preferred_height));
      }
}

void dooble_popup_menu::set_accept_on_click(bool state)
{
  m_accept_on_click = state;
}

void dooble_popup_menu::slot_authenticated(bool state)
{
  if(!dooble_settings::has_dooble_credentials())
    m_ui.authenticate->setEnabled(false);
  else
    m_ui.authenticate->setEnabled(!state);
}

void dooble_popup_menu::slot_settings_applied(void)
{
  if(!dooble_settings::has_dooble_credentials())
    m_ui.authenticate->setToolTip
      (tr("Permanent credentials have not been prepared."));
  else
    m_ui.authenticate->setToolTip("");

  prepare_icons();
}

void dooble_popup_menu::slot_tool_button_clicked(void)
{
  if(m_ui.blocked_domains == sender())
    {
      emit show_accepted_or_blocked_domains();

      if(m_accept_on_click)
	accept();
    }
  else if(m_ui.clear_items == sender())
    {
      dooble_clear_items clear_items(find_parent_dooble());

      clear_items.exec();
      QApplication::processEvents();
    }
  else if(m_ui.cookies == sender())
    {
      emit show_cookies();

      if(m_accept_on_click)
	accept();
    }
  else if(m_ui.exit_dooble == sender())
    emit quit_dooble();
  else if(m_ui.history == sender())
    {
      emit show_history();

      if(m_accept_on_click)
	accept();
    }
  else if(m_ui.new_private_window == sender())
    (new dooble(QList<QUrl> () << QUrl(), true))->show();
  else if(m_ui.new_tab == sender())
    {
      auto d = find_parent_dooble();

      if(d)
	d->new_page(QUrl(), d->is_private());

      if(m_accept_on_click)
	accept();
    }
  else if(m_ui.new_window == sender())
    (new dooble(QList<QUrl> () << QUrl(), false))->show();
  else if(m_ui.print == sender())
    {
      auto d = find_parent_dooble();

      if(d)
	d->print_current_page();
    }
  else if(m_ui.save_page == sender())
    {
      emit save();

      if(m_accept_on_click)
	accept();
    }
  else if(m_ui.settings == sender())
    {
      emit show_settings();

      if(m_accept_on_click)
	accept();
    }
  else if(m_ui.zoom_in == sender())
    emit zoom_in();
  else if(m_ui.zoom_out == sender())
    emit zoom_out();
  else if(m_ui.zoom_reset == sender())
    emit zoom_reset();
}

void dooble_popup_menu::slot_zoomed(qreal zoom_factor)
{
  m_ui.zoom_reset->setText
    (tr("%1%").
     arg(QString::
	 number(static_cast<int> (100 * qBound(0.25, zoom_factor, 5.0)))));
}
