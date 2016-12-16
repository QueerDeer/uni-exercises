#ifndef PLUGINFACTORY_H
#define PLUGINFACTORY_H

#include <QWebPluginFactory>
#include <QList>
#include <QWebView>
#include "diagrammwidget.h"

class pluginFactory : public QWebPluginFactory
{
public:
    pluginFactory(QWebPage*);
    QList<Plugin> plugins() const;
    QObject* create(const QString &mimeType, const QUrl &, const QStringList &argumentNames, const QStringList &argumentValues) const;
};

#endif // PLUGINFACTORY_H
