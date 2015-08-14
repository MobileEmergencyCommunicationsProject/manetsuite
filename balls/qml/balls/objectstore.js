.pragma library

var uuidToObject = new Array()

function get(key) {
    return uuidToObject[key]
}

// Associate the key with an object, obj.
// BUG: can leak objects by using the same key for different objects
function insert(key, obj) {
    uuidToObject[key] = obj
}

// Remove the object associated with key.
// Return the object.
function remove(key) {
    var obj = uuidToObject[key]
    uuidToObject[key] = null
    return obj
}
