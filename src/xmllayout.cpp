#include "xmllayout.h"
#include "loggingevent.h"

#include <QDebug>
#include <QXmlStreamWriter>

namespace Log4Qt
{

XMLLayout::XMLLayout(QObject *pParent)
    : Layout(pParent)

{
}

XMLLayout::~XMLLayout()
{
}

QString XMLLayout::format(const LoggingEvent &rEvent)
{
    QString output;
    QXmlStreamWriter writer(&output);

    writer.writeStartElement("log4j:event");
    writer.writeAttribute("logger", rEvent.loggerName());
    writer.writeAttribute("timestamp", QString::number(rEvent.timeStamp()));
    writer.writeAttribute("level", rEvent.level().toString());
    writer.writeAttribute("thread", rEvent.threadName());

    writer.writeStartElement("log4j:message");
    writer.writeCDATA(rEvent.message());
    writer.writeEndElement();

    if (!rEvent.ndc().isEmpty())
    {
        writer.writeStartElement("log4j:NDC");
        writer.writeCDATA(rEvent.ndc());
        writer.writeEndElement();
    }

    auto props = rEvent.properties();
    if (!props.isEmpty())
    {
        writer.writeStartElement("log4j:properties");
        for (auto pos = props.constBegin(); pos != props.constEnd(); ++pos)
        {
            writer.writeStartElement("log4j:data");
            writer.writeAttribute("name", pos.key());
            writer.writeAttribute("value", pos.value());
            writer.writeEndElement();
        }
        writer.writeEndElement();
    }
    writer.writeEndElement();

    return output;
}

#ifndef QT_NO_DEBUG_STREAM
QDebug XMLLayout::debug(QDebug &rDebug) const
{
    rDebug.nospace() << "XMLLayout("
                     << "name:" << name() << " "
                     << ")";
    return rDebug.space();
}
#endif // QT_NO_DEBUG_STREAM

}

#include "moc_xmllayout.cpp"
