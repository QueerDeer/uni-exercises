#include "pluginfactory.h"

pluginFactory::pluginFactory(QWebPage *)
{

}

QList<QWebPluginFactory::Plugin> pluginFactory::plugins() const
{
    MimeType mimetype;
    mimetype.name = "application/x-chart";
    mimetype.description = "showing diagramms";

    Plugin plugin;
    plugin.name = "diagramm";
    plugin.description = "draw the pie";
    plugin.mimeTypes = QList<MimeType>() << mimetype;
    return QList<Plugin>() << plugin;
}

QObject *pluginFactory::create(const QString &mimeType, const QUrl &url,
                            const QStringList &argumentNames,
                            const QStringList &argumentValues) const
{
    if (mimeType == "application/x-chart")
        return new diagrammWidget(argumentValues[0], argumentValues[1].toInt());
    else
        return NULL;
}
