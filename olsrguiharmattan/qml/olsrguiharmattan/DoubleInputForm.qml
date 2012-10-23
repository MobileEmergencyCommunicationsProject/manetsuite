// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

TextInputForm {
    id: container

    /*
      This DoubleValidator uses the default
      values of bottom, decimals, and top.

      Supply your own DoubleValidator to DecoratedTextInput
      if you need to validate a different range or precision.
      */
    validator: DoubleValidator {
        id: doubleValidator
        notation: DoubleValidator.StandardNotation
    }
}
