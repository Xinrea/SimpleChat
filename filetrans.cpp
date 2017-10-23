#include "filetrans.h"
#include <QFileInfo>

fileTrans::fileTrans(QString paths, unsigned long ips, unsigned ports):window(5),path(paths),ip(ips),port(ports)
{
    fileSocket.config(ips,ports);
    headSocket.config(ips,ports);
    fileInfo(path);
}

void fileTrans::fileInfo(QString path)
{
    fileHead filehead;
    fileAck ack;
    QFileInfo fileInfo(path);
    strcpy(filehead.filename,fileInfo.baseName().toStdString().data());
    filehead.size = fileInfo.size();
    filehead.total = fileInfo.size()/1020;
    headSocket.connectToHost();
    headSocket.sendMsg(reinterpret_cast<char*>(&filehead),sizeof(fileHead));
    headSocket.disconnect();
    return;
}
