#include <iostream>
#include <QtCore/QCoreApplication>
#include <receiver.h>
#include <sender.h>

int main(int argc, char *argv[])
{
    int answer = -1;
    QCoreApplication a(argc, argv);

    if (argc > 2) {
        if (0 == strcmp("-r", argv[1])) {
            // receiver
            Receiver receiver(argv[2]);
            answer = a.exec();
        } else if (0 == strcmp("-s", argv[1])) {
            // sender
            Sender sender(argv[2]);
            answer = a.exec();
        } else {
            std::cout << "Usage: qpipetest -r|-s pipe-name" << std::endl;
        }
    } else {
        std::cout << "Usage: qpipetest -r|-s pipe-name" << std::endl;
    }

    return answer;
}
