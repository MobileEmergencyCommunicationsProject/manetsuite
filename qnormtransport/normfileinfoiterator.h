#ifndef NORMFILEINFOITERATOR_H
#define NORMFILEINFOITERATOR_H

#include <QDirIterator>
#include <QList>
#include <QString>

/*
  NormFileInfoIterator is a support class for QNormFileTransport. It is a
  queue of file names and directory names along with a way to iterate over them.
  For each file, it computes the remote file name and path.
  */
class NormFileInfoIterator
{
public:
    NormFileInfoIterator();
    ~NormFileInfoIterator();

    // Add a file or directory to the end of the queue.
    void append(const QString & name);

    // Returns the QString most recently returned by next();
    QString current();

    // Returns true if there is a next element in the iterator. Returns false
    // otherwise.
    bool hasNext();

    // Returns the name for current() at the destination.
    // There are two rules for computing info(). They depend
    // on how the NormFileInfoIterator obtained current().
    //
    // 1. If current() was append()ed as the name of a file
    //    with or without a path, info() is the name of the
    //    file without any path.
    //
    // 2. If current() came from a directory path that was append()ed
    //    then info() is the file name along with the last directory
    //    in that directory path.
    //
    // Examples.
    //
    // 1. /tmp/foo.txt
    //    info() is foo.txt.
    //
    // 2. /etc
    //    info() for any file begins with etc, so for
    //    /etc/hosts, info() is etc/hosts.
    //
    // 3. /var/log
    //    For a file in /var/log, info() begins with log, so
    //    for /var/log/messages, info() is log/messages.
    //    For a file in a subdirectory of /var/log, info() also
    //    begins with log, so info() for /var/log/apt/history.log
    //    is log/apt/history.log.
    //
    // 4. ghostscript, a subdirectory of /etc
    //    info() begins with ghostscript, so
    //    info() for /etc/ghostscript/cidfmap.d/90gs-cjk-resource-cns1.conf
    //    is ghostscript/cidfmap.d/90gs-cjk-resource-cns1.conf.
    //
    // 5. ghostscript/*, files and subdirectories of /etc
    //    info() begins with ghostscript, as above.
    //
    QString info();

    // Advances the iterator and returns the path to the file at the head
    // of the queue.
    QString next();

private:
    QList<QString> _queue;
    QDirIterator *_directoryIterator;
    QString _current;
};

#endif // NORMFILEINFOITERATOR_H
