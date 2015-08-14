#include <iostream>
#include "normfileinfoiterator.h"
#include <QFileInfo>

/*
  Test NormFileInfoIterator.

  Usage: NormFileInfoIteratortest [files]

  Iterate over the files and directories named on the command line. If there
  are no command line arguments, then iterate over the current directory.

  */

void testQDirIterator(int argc, char *argv[])
{
    if (argc == 1) {
        QDirIterator dirIterator(".",
                     QDir::Files|QDir::NoSymLinks|QDir::Readable,
                     QDirIterator::Subdirectories);
        while (dirIterator.hasNext()) {
            QString name = dirIterator.next();
            std::cout << name.toStdString() << std::endl;
        }
    } else {
        for (int i=1; i < argc; i++) {
            QFileInfo info(argv[i]);
            if (info.isDir()) {
                QDirIterator dirIterator(argv[i],
                             QDir::Files|QDir::NoSymLinks|QDir::Readable,
                             QDirIterator::Subdirectories);
                while (dirIterator.hasNext()) {
                    QString name = dirIterator.next();
                    std::cout << name.toStdString() << std::endl;
                }
            } else {
                std::cout << argv[i] << std::endl;
            }
        }
    }
}

void testNormFileInfoIterator(int argc, char *argv[])
{
    NormFileInfoIterator iterator;

    if (1 == argc) {
        iterator.append(".");
    } else {
        for(int i=1; i < argc; i++) {
            iterator.append(argv[i]);
        }
    }
    while (iterator.hasNext()) {
        QString name = iterator.next();
        std::cout << name.toStdString() << std::endl;
    }
}

void testInfo(int argc, char *argv[])
{
    NormFileInfoIterator iterator;

    if (1 == argc) {
        iterator.append(".");
    } else {
        for(int i=1; i < argc; i++) {
            iterator.append(argv[i]);
        }
    }
    while (iterator.hasNext()) {
        QString name = iterator.next();
        std::cout << "Current: " <<
                     iterator.current().toStdString() << std::endl;
        std::cout << "Info   : " <<
                     iterator.info().toStdString() << std::endl;
    }
}

int main(int argc, char *argv[])
{
    std::cout << "QDirIterator says:" << std::endl;
    testQDirIterator(argc, argv);

    std::cout << std::endl;
    std::cout << "FileAndFileInfoIterator says:" << std::endl;
    testNormFileInfoIterator(argc, argv);

    std::cout << std::endl;
    std::cout << "testInfo says:" << std::endl;
    testInfo(argc, argv);
}
