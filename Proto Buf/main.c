#include <stdio.h>
#include <stdlib.h>
#include "person.pb-c.h"


int main() {
    // Step 1: Create a Person message;
    Person person = PERSON__INIT;
    person.name = "Oggy";
    person.age = 25;
    person.is_student = 1;

    // Step 2: Serialize the DATA

    // Get size of the serialized message
    size_t size = person__get_packed_size(&person);

    // Allocate memory for the buffer
    void *buffer = malloc(size);

    // Serialize (pack) the Person into the buffer
    person__pack(&person, buffer);

    // Print the serialized data
    printf("Serialized data: ");
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", ((uint8_t *)buffer)[i]);
    }
    printf("\n");

    // Step 3: Deserialize the binary data => real data
    Person *unpacked_person = person__unpack(NULL, size, buffer);  // Unpack the buffer

    if (unpacked_person == NULL) {
        printf("Error unpacking message.\n");
        return 1;  // Exit with error code
    }

    // Step 4: Access the DATA
    printf("Deserialized data:\n");
    printf("Name: %s\n", unpacked_person->name);
    printf("Age: %d\n", unpacked_person->age);
    printf("Is Student: %d\n", unpacked_person->is_student);

    return 0;
}
