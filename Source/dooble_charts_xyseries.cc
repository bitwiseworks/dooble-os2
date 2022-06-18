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

#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "dooble_charts_property_editor_xyseries.h"
#include "dooble_charts_xyseries.h"
#include "dooble_database_utilities.h"
#include "dooble_settings.h"

const QString dooble_charts_xyseries::s_chart_properties_strings[] =
  {
   tr("Color"),
   tr("Name"),
   tr("Opacity"),
   tr("Points Visible"),
   tr("Point Labels Clipping"),
   tr("Point Labels Color"),
   tr("Point Labels Font"),
   tr("Point Labels Format"),
   tr("Point Labels Visible"),
   tr("Use OpenGL"),
   tr("Visible"),
   tr("X-Axis"),
   tr("Label Format"),
   tr("Maximum"),
   tr("Minimum"),
   tr("Minor Tick Count"),
   tr("Tick Anchor"),
   tr("Tick Count"),
   tr("Tick Interval"),
   tr("Tick Type"),
   tr("Y-Axis"),
   tr("Label Format"),
   tr("Maximum"),
   tr("Minimum"),
   tr("Minor Tick Count"),
   tr("Tick Anchor"),
   tr("Tick Count"),
   tr("Tick Interval"),
   tr("Tick Type"),
   ""
  };

dooble_charts_xyseries::dooble_charts_xyseries(QWidget *parent):
  dooble_charts(parent)
{
#ifdef DOOBLE_QTCHARTS_PRESENT
  m_chart->addAxis(m_x_axis = new QValueAxis(this), Qt::AlignBottom);
  m_chart->addAxis(m_y_axis = new QValueAxis(this), Qt::AlignLeft);

  auto series = new QScatterSeries(this);

  m_series[0] = series;
  m_chart->addSeries(series);
#endif
  m_property_editor = new dooble_charts_property_editor_xyseries
    (m_ui.properties, this);
#ifdef DOOBLE_QTCHARTS_PRESENT
  series->attachAxis(m_x_axis);
  series->attachAxis(m_y_axis);
#endif
  connect(m_property_editor->model(),
	  SIGNAL(itemChanged(QStandardItem *)),
	  this,
	  SLOT(slot_item_changed(QStandardItem *)));
}

dooble_charts_xyseries::~dooble_charts_xyseries()
{
}

QHash<dooble_charts::Properties, QVariant> dooble_charts_xyseries::
properties(void) const
{
  auto properties(dooble_charts::properties());

#ifdef DOOBLE_QTCHARTS_PRESENT
  auto series = qobject_cast<QScatterSeries *> (m_series.value(0, nullptr));
  auto x_axis = qobject_cast<QValueAxis *> (m_x_axis);
  auto y_axis = qobject_cast<QValueAxis *> (m_y_axis);

  if(!series || !x_axis || !y_axis)
    return properties;

  properties[dooble_charts::XY_SERIES_COLOR] = series->color();
  properties[dooble_charts::XY_SERIES_NAME] = series->name();
  properties[dooble_charts::XY_SERIES_OPACITY] = series->opacity();
  properties[dooble_charts::XY_SERIES_POINTS_VISIBLE] = series->pointsVisible();
  properties[dooble_charts::XY_SERIES_POINT_LABELS_CLIPPING] =
    series->pointLabelsClipping();
  properties[dooble_charts::XY_SERIES_POINT_LABELS_COLOR] =
    series->pointLabelsColor();
  properties[dooble_charts::XY_SERIES_POINT_LABELS_FONT] =
    series->pointLabelsFont();
  properties[dooble_charts::XY_SERIES_POINT_LABELS_FORMAT] =
    series->pointLabelsFormat();
  properties[dooble_charts::XY_SERIES_POINT_LABELS_VISIBLE] =
    series->pointLabelsVisible();
  properties[dooble_charts::XY_SERIES_USE_OPENGL] = series->useOpenGL();
  properties[dooble_charts::XY_SERIES_VISIBLE] = series->isVisible();
  properties[dooble_charts::XY_SERIES_X_AXIS_LABEL_FORMAT] = x_axis->
    labelFormat();
  properties[dooble_charts::XY_SERIES_X_AXIS_MAX] = x_axis->max();
  properties[dooble_charts::XY_SERIES_X_AXIS_MIN] = x_axis->min();
  properties[dooble_charts::XY_SERIES_X_AXIS_MINOR_TICK_COUNT] = x_axis->
    minorTickCount();
  properties[dooble_charts::XY_SERIES_X_AXIS_TICK_ANCHOR] = x_axis->
    tickAnchor();
  properties[dooble_charts::XY_SERIES_X_AXIS_TICK_COUNT] = x_axis->tickCount();
  properties[dooble_charts::XY_SERIES_X_AXIS_TICK_INTERVAL] = x_axis->
    tickInterval();
  properties[dooble_charts::XY_SERIES_X_AXIS_TICK_TYPE] = tick_type_to_string
    (x_axis->tickType());
  properties[dooble_charts::XY_SERIES_Y_AXIS_LABEL_FORMAT] = y_axis->
    labelFormat();
  properties[dooble_charts::XY_SERIES_Y_AXIS_MAX] = y_axis->max();
  properties[dooble_charts::XY_SERIES_Y_AXIS_MIN] = y_axis->min();
  properties[dooble_charts::XY_SERIES_Y_AXIS_MINOR_TICK_COUNT] = y_axis->
    minorTickCount();
  properties[dooble_charts::XY_SERIES_Y_AXIS_TICK_ANCHOR] = y_axis->
    tickAnchor();
  properties[dooble_charts::XY_SERIES_Y_AXIS_TICK_COUNT] = y_axis->tickCount();
  properties[dooble_charts::XY_SERIES_Y_AXIS_TICK_INTERVAL] = y_axis->
    tickInterval();
  properties[dooble_charts::XY_SERIES_Y_AXIS_TICK_TYPE] = tick_type_to_string
    (y_axis->tickType());
#endif
  return properties;
}

QHash<QString, QVariant> dooble_charts_xyseries::
properties_for_database(void) const
{
  QHash<QString, QVariant> hash;
  QHashIterator<dooble_charts::Properties, QVariant> it(properties());

  while(it.hasNext())
    {
      it.next();

      if(dooble_charts::properties().contains(it.key()))
	/*
	** Ignore properties of the base class.
	*/

	continue;
      else if(!(it.key() >= dooble_charts::Properties::XY_SERIES_COLOR &&
		it.key() <= dooble_charts::Properties::XY_SERIES_VISIBLE))
	/*
	** Ignore non-chart properties.
	*/

	continue;

      auto property(property_to_name(it.key()));

      if(!property.isEmpty())
	hash[property] = it.value();
    }

  return hash;
}

QHash<QString, QVariant> dooble_charts_xyseries::
x_axis_properties_for_database(void) const
{
  QHash<QString, QVariant> hash;
  QHashIterator<dooble_charts::Properties, QVariant> it(properties());

  while(it.hasNext())
    {
      it.next();

      if(dooble_charts::properties().contains(it.key()))
	/*
	** Ignore properties of the base class.
	*/

	continue;
      else if(!(it.key() >=
		dooble_charts::Properties::XY_SERIES_X_AXIS_LABEL_FORMAT &&
		it.key() <=
		dooble_charts::Properties::XY_SERIES_X_AXIS_TICK_TYPE))
	/*
	** Ignore non-x-axis properties.
	*/

	continue;

      auto property(property_to_name(it.key()));

      if(!property.isEmpty())
	hash[property] = it.value();
    }

  return hash;
}

QHash<QString, QVariant> dooble_charts_xyseries::
y_axis_properties_for_database(void) const
{
  QHash<QString, QVariant> hash;
  QHashIterator<dooble_charts::Properties, QVariant> it(properties());

  while(it.hasNext())
    {
      it.next();

      if(dooble_charts::properties().contains(it.key()))
	/*
	** Ignore properties of the base class.
	*/

	continue;
      else if(!(it.key() >=
		dooble_charts::Properties::XY_SERIES_Y_AXIS_LABEL_FORMAT &&
		it.key() <=
		dooble_charts::Properties::XY_SERIES_Y_AXIS_TICK_TYPE))
	/*
	** Ignore non-y-axis properties.
	*/

	continue;

      auto property(property_to_name(it.key()));

      if(!property.isEmpty())
	hash[property] = it.value();
    }

  return hash;
}

QString dooble_charts_xyseries::property_to_name
(const dooble_charts::Properties property)
{
  auto name(dooble_charts::property_to_name(property).trimmed());

  if(!name.isEmpty())
    return name;

  switch(property)
    {
    case dooble_charts::Properties::XY_SERIES_COLOR:
    case dooble_charts::Properties::XY_SERIES_NAME:
    case dooble_charts::Properties::XY_SERIES_OPACITY:
    case dooble_charts::Properties::XY_SERIES_POINTS_VISIBLE:
    case dooble_charts::Properties::XY_SERIES_POINT_LABELS_CLIPPING:
    case dooble_charts::Properties::XY_SERIES_POINT_LABELS_COLOR:
    case dooble_charts::Properties::XY_SERIES_POINT_LABELS_FONT:
    case dooble_charts::Properties::XY_SERIES_POINT_LABELS_FORMAT:
    case dooble_charts::Properties::XY_SERIES_POINT_LABELS_VISIBLE:
    case dooble_charts::Properties::XY_SERIES_USE_OPENGL:
    case dooble_charts::Properties::XY_SERIES_VISIBLE:
    case dooble_charts::Properties::XY_SERIES_X_AXIS:
    case dooble_charts::Properties::XY_SERIES_X_AXIS_LABEL_FORMAT:
    case dooble_charts::Properties::XY_SERIES_X_AXIS_MAX:
    case dooble_charts::Properties::XY_SERIES_X_AXIS_MIN:
    case dooble_charts::Properties::XY_SERIES_X_AXIS_MINOR_TICK_COUNT:
    case dooble_charts::Properties::XY_SERIES_X_AXIS_TICK_ANCHOR:
    case dooble_charts::Properties::XY_SERIES_X_AXIS_TICK_COUNT:
    case dooble_charts::Properties::XY_SERIES_X_AXIS_TICK_INTERVAL:
    case dooble_charts::Properties::XY_SERIES_X_AXIS_TICK_TYPE:
    case dooble_charts::Properties::XY_SERIES_Y_AXIS:
    case dooble_charts::Properties::XY_SERIES_Y_AXIS_LABEL_FORMAT:
    case dooble_charts::Properties::XY_SERIES_Y_AXIS_MAX:
    case dooble_charts::Properties::XY_SERIES_Y_AXIS_MIN:
    case dooble_charts::Properties::XY_SERIES_Y_AXIS_MINOR_TICK_COUNT:
    case dooble_charts::Properties::XY_SERIES_Y_AXIS_TICK_ANCHOR:
    case dooble_charts::Properties::XY_SERIES_Y_AXIS_TICK_COUNT:
    case dooble_charts::Properties::XY_SERIES_Y_AXIS_TICK_INTERVAL:
    case dooble_charts::Properties::XY_SERIES_Y_AXIS_TICK_TYPE:
      {
	return s_chart_properties_strings[property - LEGEND_VISIBLE - 1];
      }
    default:
      {
	break;
      }
    }

  return "";
}

#ifdef DOOBLE_QTCHARTS_PRESENT
QString dooble_charts_xyseries::tick_type_to_string
(const QValueAxis::TickType tick_type)
{
  switch(tick_type)
    {
    case QValueAxis::TicksDynamic:
      {
	return tr("Dynamic");
      }
    default:
      {
	return tr("Fixed");
      }
    }
}

QValueAxis::TickType dooble_charts_xyseries::string_to_tick_type
(const QString &t)
{
  auto text(t.trimmed());

  if(text == tr("Dynamic"))
    return QValueAxis::TicksDynamic;
  else
    return QValueAxis::TicksFixed;
}
#endif

void dooble_charts_xyseries::open(const QString &name)
{
  dooble_charts::open(name);
}

void dooble_charts_xyseries::save(QString &error)
{
  dooble_charts::save(error);

  if(!error.isEmpty())
    return;

  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

  auto database_name(dooble_database_utilities::database_name());

  {
    auto db = QSqlDatabase::addDatabase("QSQLITE", database_name);

    db.setDatabaseName(dooble_settings::setting("home_path").toString() +
		       QDir::separator() +
		       "dooble_charts.db");

    if(db.open())
      {
	QSqlQuery query(db);
	auto name(properties().value(dooble_charts::Properties::CHART_NAME).
		  toString().toUtf8());

	query.prepare("INSERT INTO dooble_charts "
		      "(name, property, subset_name, value) "
		      "VALUES (?, ?, ?, ?)");
	query.addBindValue(name.toBase64());
	query.addBindValue(QString("chart_type").toUtf8().toBase64());
	query.addBindValue("chart_type");
	query.addBindValue(QString("xyseries").toUtf8().toBase64());
	query.exec();

	{
	  QHashIterator<QString, QVariant> it(properties_for_database());

	  while(it.hasNext())
	    {
	      it.next();
	      query.prepare
		("INSERT OR REPLACE INTO dooble_charts "
		 "(name, property, subset_name, value) "
		 "VALUES (?, ?, ?, ?)");
	      query.addBindValue(name.toBase64());
	      query.addBindValue(it.key().toUtf8().toBase64());
	      query.addBindValue("xyseries_properties");
	      query.addBindValue(it.value().toString().toUtf8().toBase64());
	      query.exec();
	    }
	}

	{
	  QHashIterator<QString, QVariant> it
	    (x_axis_properties_for_database());

	  while(it.hasNext())
	    {
	      it.next();
	      query.prepare
		("INSERT OR REPLACE INTO dooble_charts "
		 "(name, property, subset_name, value) "
		 "VALUES (?, ?, ?, ?)");
	      query.addBindValue(name.toBase64());
	      query.addBindValue(it.key().toUtf8().toBase64());
	      query.addBindValue("xyseries_x_axis_properties");
	      query.addBindValue(it.value().toString().toUtf8().toBase64());
	      query.exec();
	    }
	}

	{
	  QHashIterator<QString, QVariant> it
	    (y_axis_properties_for_database());

	  while(it.hasNext())
	    {
	      it.next();
	      query.prepare
		("INSERT OR REPLACE INTO dooble_charts "
		 "(name, property, subset_name, value) "
		 "VALUES (?, ?, ?, ?)");
	      query.addBindValue(name.toBase64());
	      query.addBindValue(it.key().toUtf8().toBase64());
	      query.addBindValue("xyseries_y_axis_properties");
	      query.addBindValue(it.value().toString().toUtf8().toBase64());
	      query.exec();
	    }
	}
      }

    db.close();
  }

  QSqlDatabase::removeDatabase(database_name);
  QApplication::restoreOverrideCursor();
}

void dooble_charts_xyseries::slot_clear(void)
{
#ifdef DOOBLE_QTCHARTS_PRESENT
  QMapIterator<int, QPointer<QAbstractSeries> > it(m_series);

  while(it.hasNext())
    {
      it.next();

      auto series = qobject_cast<QScatterSeries *> (it.value());

      if(series)
	series->clear();
    }
#endif
}

void dooble_charts_xyseries::slot_data_ready
(const QVector<double> &vector, const int index)
{
  if(vector.size() != 2)
    return;

#ifdef DOOBLE_QTCHARTS_PRESENT
  auto series = qobject_cast<QScatterSeries *> (m_series.value(index, nullptr));

  if(!series)
    return;

  auto x = vector.at(0);
  auto y = vector.at(1);

  series->append(x, y);

  /*
  ** Automatically adjust the property editor.
  */

  auto x_axis = qobject_cast<QValueAxis *> (m_x_axis);
  auto y_axis = qobject_cast<QValueAxis *> (m_y_axis);

  if(!x_axis || !y_axis)
    return;

  auto set = false;

  if(x <= x_axis->min())
    {
      set = true;
      x_axis->setMin(x - 1.0);
    }

  if(x >= x_axis->max())
    {
      set = true;
      x_axis->setMax(x + 1.0);
    }

  if(y <= y_axis->min())
    {
      set = true;
      y_axis->setMin(y - 1.0);
    }

  if(y >= y_axis->max())
    {
      set = true;
      y_axis->setMax(y + 1.0);
    }

  if(m_property_editor && set)
    {
      m_property_editor->set_property
	(dooble_charts::Properties::XY_SERIES_X_AXIS_MAX, x_axis->max());
      m_property_editor->set_property
	(dooble_charts::Properties::XY_SERIES_X_AXIS_MIN, x_axis->min());
      m_property_editor->set_property
	(dooble_charts::Properties::XY_SERIES_Y_AXIS_MAX, y_axis->max());
      m_property_editor->set_property
	(dooble_charts::Properties::XY_SERIES_Y_AXIS_MIN, y_axis->min());
    }
#else
  Q_UNUSED(index);
#endif
}

void dooble_charts_xyseries::slot_item_changed(QStandardItem *item)
{
  if(!item)
    return;

  dooble_charts::slot_item_changed(item);

#ifdef DOOBLE_QTCHARTS_PRESENT
  auto series = qobject_cast<QScatterSeries *> (m_series.value(0, nullptr));
  auto x_axis = qobject_cast<QValueAxis *> (m_x_axis);
  auto y_axis = qobject_cast<QValueAxis *> (m_y_axis);

  if(!series || !x_axis || !y_axis)
    return;

  auto property = dooble_charts::Properties
    (item->data(Qt::ItemDataRole(Qt::UserRole + 1)).toInt());

  switch(property)
    {
    case dooble_charts::Properties::XY_SERIES_COLOR:
      {
	series->setColor(QColor(item->text()));
	break;
      }
    case dooble_charts::Properties::XY_SERIES_NAME:
      {
	series->setName(item->text());
	break;
      }
    case dooble_charts::Properties::XY_SERIES_OPACITY:
      {
	series->setOpacity(item->text().toDouble());
	break;
      }
    case dooble_charts::Properties::XY_SERIES_POINTS_VISIBLE:
      {
	series->setPointsVisible(item->checkState() == Qt::Checked);
	break;
      }
    case dooble_charts::Properties::XY_SERIES_POINT_LABELS_CLIPPING:
      {
	series->setPointLabelsClipping(item->checkState() == Qt::Checked);
	break;
      }
    case dooble_charts::Properties::XY_SERIES_POINT_LABELS_COLOR:
      {
	series->setPointLabelsColor(QColor(item->text()));
	break;
      }
    case dooble_charts::Properties::XY_SERIES_POINT_LABELS_FONT:
      {
	QFont font;

	if(!item->text().trimmed().isEmpty() &&
	   font.fromString(item->text().trimmed()))
	  series->setPointLabelsFont(font);
	else
	  series->setPointLabelsFont(QApplication::font());

	break;
      }
    case dooble_charts::Properties::XY_SERIES_POINT_LABELS_FORMAT:
      {
	series->setPointLabelsFormat(item->text());
	break;
      }
    case dooble_charts::Properties::XY_SERIES_POINT_LABELS_VISIBLE:
      {
	series->setPointLabelsVisible(item->checkState() == Qt::Checked);
	break;
      }
    case dooble_charts::Properties::XY_SERIES_USE_OPENGL:
      {
	series->setUseOpenGL(item->checkState() == Qt::Checked);
	break;
      }
    case dooble_charts::Properties::XY_SERIES_VISIBLE:
      {
	series->setVisible(item->checkState() == Qt::Checked);
	break;
      }
    case dooble_charts::Properties::XY_SERIES_X_AXIS_LABEL_FORMAT:
      {
	x_axis->setLabelFormat(item->text());
	break;
      }
    case dooble_charts::Properties::XY_SERIES_X_AXIS_MAX:
      {
	x_axis->setMax(item->text().toDouble());
	break;
      }
    case dooble_charts::Properties::XY_SERIES_X_AXIS_MIN:
      {
	x_axis->setMin(item->text().toDouble());
	break;
      }
    case dooble_charts::Properties::XY_SERIES_X_AXIS_MINOR_TICK_COUNT:
      {
	x_axis->setMinorTickCount(item->text().toInt());
	break;
      }
    case dooble_charts::Properties::XY_SERIES_X_AXIS_TICK_ANCHOR:
      {
	x_axis->setTickAnchor(item->text().toDouble());
	break;
      }
    case dooble_charts::Properties::XY_SERIES_X_AXIS_TICK_COUNT:
      {
	x_axis->setTickCount(item->text().toInt());
	break;
      }
    case dooble_charts::Properties::XY_SERIES_X_AXIS_TICK_INTERVAL:
      {
	x_axis->setTickInterval(qMax(0.1, item->text().toDouble()));
	break;
      }
    case dooble_charts::Properties::XY_SERIES_X_AXIS_TICK_TYPE:
      {
	x_axis->setTickType(string_to_tick_type(item->text()));
	break;
      }
    case dooble_charts::Properties::XY_SERIES_Y_AXIS_LABEL_FORMAT:
      {
	y_axis->setLabelFormat(item->text());
	break;
      }
    case dooble_charts::Properties::XY_SERIES_Y_AXIS_MAX:
      {
	y_axis->setMax(item->text().toDouble());
	break;
      }
    case dooble_charts::Properties::XY_SERIES_Y_AXIS_MIN:
      {
	y_axis->setMin(item->text().toDouble());
	break;
      }
    case dooble_charts::Properties::XY_SERIES_Y_AXIS_MINOR_TICK_COUNT:
      {
	y_axis->setMinorTickCount(item->text().toInt());
	break;
      }
    case dooble_charts::Properties::XY_SERIES_Y_AXIS_TICK_ANCHOR:
      {
	y_axis->setTickAnchor(item->text().toDouble());
	break;
      }
    case dooble_charts::Properties::XY_SERIES_Y_AXIS_TICK_COUNT:
      {
	y_axis->setTickCount(item->text().toInt());
	break;
      }
    case dooble_charts::Properties::XY_SERIES_Y_AXIS_TICK_INTERVAL:
      {
	y_axis->setTickInterval(qMax(0.1, item->text().toDouble()));
	break;
      }
    case dooble_charts::Properties::XY_SERIES_Y_AXIS_TICK_TYPE:
      {
	y_axis->setTickType(string_to_tick_type(item->text()));
	break;
      }
    default:
      {
	break;
      }
    }
#endif
}
