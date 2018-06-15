#ifndef FILESPATH_H
#define FILESPATH_H

#include <QString>
#include <QStringList>
#include <QVector>

struct FilePath
{
    QString name;
    QString format;
    QString dir;

    void setPath(const QString &path);
    QString getFullName() const;
    void setFormat(const QString &format);
};

class FilesPath
{
public:
    FilesPath();
    FilesPath(const QStringList &filesPath);

    static QString getDir(const QString &filePath);
    static QString getName(const QString &filePath);
    static QString getFormat(const QString &filePath);
    static QVector<FilePath> getFilesPath(const QStringList &filesPath);

    FilePath getFilePath(const int &indPath) const;
    FilePath* filePath(const int &indPath);
    QStringList getFullNames() const;
    QStringList getNames() const;
    int size() const;
    void setFormats(const QString &format);

private:
    QVector<FilePath> files;
};

#endif // FILESPATH_H
