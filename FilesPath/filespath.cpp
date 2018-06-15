#include "filespath.h"


FilesPath::FilesPath()
{

}



FilesPath::FilesPath(const QStringList &filesPath)
{
    this->files = getFilesPath(filesPath);
}



QString FilesPath::getDir(const QString &filePath)
{
    QString pathLoc = filePath;
    QStringList split = pathLoc.split("/");
    QString fileName = split.last();
    QString dir = pathLoc.remove(fileName);
    return dir;
}



QString FilesPath::getName(const QString &filePath)
{
    QString path = filePath;
    QStringList pathSplit = path.split("/");
    QStringList nameWithFormat = pathSplit.last().split(".");
    QString name = nameWithFormat.first();
    return name;
}



QString FilesPath::getFormat(const QString &filePath)
{
    QString path = filePath;
    QStringList pathSplit = path.split("/");
    QStringList nameWithFormat = pathSplit.last().split(".");
    QString format = nameWithFormat.last();
    return format;
}



QVector<FilePath> FilesPath::getFilesPath(const QStringList &filesPath)
{
    QVector<FilePath> files;

    for (int indPath = 0; indPath < filesPath.size(); indPath++){
        QString fullName = filesPath[indPath];
        FilePath path;
        path.dir = getDir(fullName);
        path.name = getName(fullName);
        path.format = getFormat(fullName);
        files.push_back(path);
    }

    return files;
}



FilePath FilesPath::getFilePath(const int &indPath) const
{
    return files[indPath];
}



FilePath *FilesPath::filePath(const int &indPath)
{
    return &files[indPath];
}



QStringList FilesPath::getFullNames() const
{
    QStringList names;

    for (int indPath = 0; indPath < size(); indPath++)
        names.push_back(files[indPath].getFullName());

    return names;
}



QStringList FilesPath::getNames() const
{
    QStringList names;

    for (int indPath = 0; indPath < size(); indPath++)
        names.push_back(files[indPath].name);

    return names;
}



int FilesPath::size() const
{
    return files.size();
}



void FilesPath::setFormats(const QString &format)
{
    for (int indPath = 0; indPath < files.size(); indPath++)
        files[indPath].setFormat(format);
}



void FilePath::setPath(const QString &path)
{
    dir = FilesPath::getDir(path);
    format = FilesPath::getFormat(path);
    name = FilesPath::getName(path);
}



QString FilePath::getFullName() const
{
    return dir + name + '.' + format;
}



void FilePath::setFormat(const QString &format)
{
    this->format = format;
}
