#ifndef AUTHORITY_H
#define AUTHORITY_H

#include <QString>

class Authority
{
public:  
    // Displays a dialog box that asks for permission to rename
    // a file.  Blocks until there is an answer.
    //
    // Returns true if permission was granted. Returns
    // false otherwise.
    virtual bool askToRenameFile(QString oldFileName, QString newFileName) = 0;
};

#endif // AUTHORITY_H
