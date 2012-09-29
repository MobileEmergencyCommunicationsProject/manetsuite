#include "normfileinfoiterator.h"
#include <QDebug>

NormFileInfoIterator::NormFileInfoIterator() : _directoryIterator(0)
{
}

NormFileInfoIterator::~NormFileInfoIterator()
{
    delete _directoryIterator;
}

// Add a file or directory to the end of the queue.
void NormFileInfoIterator::append(const QString & name)
{
    _queue.append(name);
}

QString NormFileInfoIterator::current()
{
    return _current;
}

// Returns true if there is a next element in the iterator. Returns false
// otherwise.
bool NormFileInfoIterator::hasNext()
{
    /*
       This FileAndInfoIterator has a next element if any of the
       following are true:

       1. _directoryIterator->hasNext()
       2. _queue contains either a file or a directory that contains files.

       This works because _directoryIterator includes only files.
      */
    bool answer = false;

    if (0 != _directoryIterator) {
        if (! (answer = _directoryIterator->hasNext())) {
            delete _directoryIterator;
            _directoryIterator = 0;
        }
    }

    while ((! answer) && (! _queue.isEmpty())) {
        QString first = _queue.first();
        QFileInfo firstInfo(first);
        if (firstInfo.isDir()) {
            _queue.removeFirst();
            _directoryIterator =
                    new QDirIterator(first,
                                     QDir::Files|QDir::NoSymLinks|QDir::Readable,
                                     QDirIterator::Subdirectories);
            if (! (answer = _directoryIterator->hasNext())) {
                delete _directoryIterator;
                _directoryIterator = 0;
            }
        } else {
            answer = firstInfo.exists() && firstInfo.isFile() && firstInfo.isReadable();
            if (!answer) {
                qWarning() << "FileAndDirectoryIterator::hasNext(): Skipping non-existent or non-file or non-readable "
                           << first;
                _queue.removeFirst();
            }
        }
    }

    return answer;
}

QString NormFileInfoIterator::info()
{
    QString answer;

    if (0 == _directoryIterator) {
        QFileInfo currentInfo(_current);
        answer = currentInfo.fileName();
    } else {
        QString baseDir = _directoryIterator->path();
        QString filePath = _directoryIterator->filePath();
        QDir base(baseDir);
        if (base.isAbsolute()) {
            answer = base.dirName().append("/").append(base.relativeFilePath(filePath));
        } else {
            answer = base.relativeFilePath(filePath);
        }
    }

    return answer;
}

// Advances the iterator and returns he full path to the file at the head
// of the queue.
QString NormFileInfoIterator::next()
{
    QString answer;
    bool useQueue = true;

    if (0 != _directoryIterator) {
        if (_directoryIterator->hasNext()) {
            answer = _current = _directoryIterator->next();
            useQueue = false;
        } else {
            delete _directoryIterator;
            _directoryIterator = 0;
        }
    }

    while (useQueue && (! _queue.isEmpty())) {
        QString first = _queue.first();
        QFileInfo firstInfo(first);
        if (firstInfo.isDir()) {
            _queue.removeFirst();
            _directoryIterator = new QDirIterator(first, QDir::Files|QDir::NoSymLinks|QDir::Readable,
                                                  QDirIterator::Subdirectories);
            if (_directoryIterator->hasNext()) {
                answer = _current = _directoryIterator->next();
                useQueue = false;
            } else {
                delete _directoryIterator;
                _directoryIterator = 0;
            }
        } else {
            useQueue = ! firstInfo.exists() || ! firstInfo.isFile() || ! firstInfo.isReadable();
            if (useQueue) {
                qWarning() << "FileAndDirectoryIterator::next(): Skipping non-existent or non-file or non-readable "
                           << first;
            } else {
                answer = _current = first;
            }

            _queue.removeFirst();
        }
    }

    return answer;
}
