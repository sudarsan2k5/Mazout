CC = gcc

# Protobuf C flags and libraries
CFLAGS = -I/usr/include/protobuf-c
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lprotobuf-c

# Proto file and generated C and header files
PROTO_FILE = person.proto
PROTO_SRC = person.pb-c.c
PROTO_HEADER = person.pb-c.h

# C source file
SRC = main.c

# Output executable
TARGET = person_program

all: $(TARGET)

# Generate C and header files from .proto
$(PROTO_SRC): $(PROTO_FILE)
	protoc --c_out=. $(PROTO_FILE)

# Compile the program
$(TARGET): $(SRC) $(PROTO_SRC)
	$(CC) -o $(TARGET) $(SRC) $(PROTO_SRC) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET) $(PROTO_SRC) $(PROTO_HEADER)
