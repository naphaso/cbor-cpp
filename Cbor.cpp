#include "Cbor.h"

#include <string.h>
#include <stdio.h>

using namespace std;

CborInput::CborInput(void *data, int size) {
	this->data = (unsigned char *)data;
	this->size = size;
	this->offset = 0;
}

CborInput::~CborInput() {

}

bool CborInput::hasBytes(int count) {
	return size - offset >= count;
}

unsigned char CborInput::getByte() {
	return data[offset++];
}

unsigned short CborInput::getShort() {
	unsigned short value = ((unsigned short)data[offset] << 8) | ((unsigned short)data[offset + 1]);
	offset += 2;
	return value;
}

unsigned int CborInput::getInt() {
	unsigned int value = ((unsigned int)data[offset] << 24) | ((unsigned int)data[offset + 1] << 16) | ((unsigned int)data[offset + 2] << 8) | ((unsigned int)data[offset + 3]);
	offset += 4;
	return value;
}

unsigned long long CborInput::getLong() {
	unsigned long long value = ((unsigned long long)data[offset] << 56) | ((unsigned long long)data[offset+1] << 48) | ((unsigned long long)data[offset+2] << 40) | ((unsigned long long)data[offset+3] << 32) | ((unsigned long long)data[offset+4] << 24) | ((unsigned long long)data[offset+5] << 16) | ((unsigned long long)data[offset+6] << 8) | ((unsigned long long)data[offset+7]); 
	offset += 8;
	return value;
}

void CborInput::getBytes(void *to, int count) {
	memcpy(to, data + offset, count);
	offset += count;
}


CborReader::CborReader(CborInput &input) {
	this->input = &input;
	this->state = STATE_TYPE;
}

CborReader::~CborReader() {

}

void CborReader::run() {
	while(1) {
		if(state == STATE_TYPE) {
			//printf("STATE_TYPE\n");
			if(input->hasBytes(1)) {
				unsigned char type = input->getByte();
				unsigned char majorType = type >> 5;
				unsigned char minorType = type & 31;

				switch(majorType) {
					case 0: // positive integer
						if(minorType < 24) {
							onInteger(minorType);
						} else if(minorType == 24) { // 1 byte
							currentLength = 1;
							state = STATE_PINT;
						} else if(minorType == 25) { // 2 byte
							currentLength = 2;
							state = STATE_PINT;
						} else if(minorType == 26) { // 4 byte
							currentLength = 4;
							state = STATE_PINT;
						} else if(minorType == 27) { // 8 byte
							currentLength = 8;
							state = STATE_PINT;
						} else {
							onError("invalid integer type");
						}
						break;
					case 1: // negative integer
						if(minorType < 24) {
							onInteger(-minorType);
						} else if(minorType == 24) { // 1 byte
							currentLength = 1;
							state = STATE_NINT;
						} else if(minorType == 25) { // 2 byte
							currentLength = 2;
							state = STATE_NINT;
						} else if(minorType == 26) { // 4 byte
							currentLength = 4;
							state = STATE_NINT;
						} else if(minorType == 27) { // 8 byte
							currentLength = 8;
							state = STATE_NINT;
						} else {
							onError("invalid integer type");
						}
						break;
					case 2: // bytes
						if(minorType < 24) {
							state = STATE_BYTES_DATA;
							currentLength = minorType;
						} else if(minorType == 24) {
							state = STATE_BYTES_SIZE;
							currentLength = 1;
						} else if(minorType == 25) { // 2 byte
							currentLength = 2;
							state = STATE_BYTES_SIZE;
						} else if(minorType == 26) { // 4 byte
							currentLength = 4;
							state = STATE_BYTES_SIZE;
						} else if(minorType == 27) { // 8 byte
							currentLength = 8;
							state = STATE_BYTES_SIZE;
						} else {
							onError("invalid bytes type");
						}
						break;
					case 3: // string
						if(minorType < 24) {
							state = STATE_STRING_DATA;
							currentLength = minorType;
						} else if(minorType == 24) {
							state = STATE_STRING_SIZE;
							currentLength = 1;
						} else if(minorType == 25) { // 2 byte
							currentLength = 2;
							state = STATE_STRING_SIZE;
						} else if(minorType == 26) { // 4 byte
							currentLength = 4;
							state = STATE_STRING_SIZE;
						} else if(minorType == 27) { // 8 byte
							currentLength = 8;
							state = STATE_STRING_SIZE;
						} else {
							onError("invalid string type");
						}
						break;
					case 4: // array
						if(minorType < 24) {
							onArray(minorType);
						} else if(minorType == 24) {
							state = STATE_ARRAY;
							currentLength = 1;
						} else if(minorType == 25) { // 2 byte
							currentLength = 2;
							state = STATE_ARRAY;
						} else if(minorType == 26) { // 4 byte
							currentLength = 4;
							state = STATE_ARRAY;
						} else if(minorType == 27) { // 8 byte
							currentLength = 8;
							state = STATE_ARRAY;
						} else {
							onError("invalid array type");
						}
						break;
					case 5: // map
						if(minorType < 24) {
							onMap(minorType);
						} else if(minorType == 24) {
							state = STATE_MAP;
							currentLength = 1;
						} else if(minorType == 25) { // 2 byte
							currentLength = 2;
							state = STATE_MAP;
						} else if(minorType == 26) { // 4 byte
							currentLength = 4;
							state = STATE_MAP;
						} else if(minorType == 27) { // 8 byte
							currentLength = 8;
							state = STATE_MAP;
						} else {
							onError("invalid array type");
						}
						break;
					case 6: // tag
						if(minorType < 24) {
							onTag(minorType);
						} else if(minorType == 24) {
							state = STATE_TAG;
							currentLength = 1;
						} else if(minorType == 25) { // 2 byte
							currentLength = 2;
							state = STATE_TAG;
						} else if(minorType == 26) { // 4 byte
							currentLength = 4;
							state = STATE_TAG;
						} else if(minorType == 27) { // 8 byte
							currentLength = 8;
							state = STATE_TAG;
						} else {
							onError("invalid tag type");
						}
						break;
					case 7: // special
						if(minorType < 24) {
							onSpecial(minorType);
						} else if(minorType == 24) {
							state = STATE_SPECIAL;
							currentLength = 1;
						} else if(minorType == 25) { // 2 byte
							currentLength = 2;
							state = STATE_SPECIAL;
						} else if(minorType == 26) { // 4 byte
							currentLength = 4;
							state = STATE_SPECIAL;
						} else if(minorType == 27) { // 8 byte
							currentLength = 8;
							state = STATE_SPECIAL;
						} else {
							onError("invalid special type");
						}
						break;
				}
			} else break;
		} else if(state == STATE_PINT) {
			//printf("STATE_PINT\n");
			if(input->hasBytes(currentLength)) {
				switch(currentLength) {
					case 1:
						onInteger(input->getByte());
						state = STATE_TYPE;
						break;
					case 2:
						onInteger(input->getShort());
						state = STATE_TYPE;
						break;
					case 4:
						// TODO: for integers > 2^31
						onInteger(input->getInt());
						state = STATE_TYPE;
						break;
					case 8:
						input->getLong();
						state = STATE_TYPE;
						// TODO: implement it
						break;
				}
			} else break;
		} else if(state == STATE_NINT) {
			//printf("STATE_NINT\n");
			if(input->hasBytes(currentLength)) {
				switch(currentLength) {
					case 1:
						onInteger(-(int)input->getByte());
						state = STATE_TYPE;
						break;
					case 2:
						onInteger(-(int)input->getShort());
						state = STATE_TYPE;
						break;
					case 4:
						// TODO: for integers > 2^31
						onInteger(-(int)input->getInt());
						state = STATE_TYPE;
						break;
					case 8:
						// TODO: implement it
						break;
				}
			} else break;
		} else if(state == STATE_BYTES_SIZE) {
			//printf("STATE_BYTES_SIZE\n");
			if(input->hasBytes(currentLength)) {
				switch(currentLength) {
					case 1:
						currentLength = input->getByte();
						state = STATE_BYTES_DATA;
						break;
					case 2:
						currentLength = input->getShort();
						state = STATE_BYTES_DATA;
						break;
					case 4:
						currentLength = input->getInt();
						state = STATE_BYTES_DATA;
						break;
					case 8:
						input->getLong();
						currentLength = 123;
						state = STATE_BYTES_DATA;
						// TODO: implement it
						break;
				}
			} else break;
		} else if(state == STATE_BYTES_DATA) {
			//printf("STATE_BYTES_DATA\n");
			if(input->hasBytes(currentLength)) {
				unsigned char *data = new unsigned char[currentLength];
				input->getBytes(data, currentLength);
				state = STATE_TYPE;
				onBytes(data, currentLength);
			} else break;
		} else if(state == STATE_STRING_SIZE) {
			//printf("STATE_STRING_SIZE\n");
			if(input->hasBytes(currentLength)) {
				switch(currentLength) {
					case 1:
						currentLength = input->getByte();
						state = STATE_STRING_DATA;
						break;
					case 2:
						currentLength = input->getShort();
						state = STATE_STRING_DATA;
						break;
					case 4:
						currentLength = input->getInt();
						state = STATE_STRING_DATA;
						break;
					case 8:
						input->getLong();
						currentLength = 123;
						state = STATE_STRING_DATA;
						// TODO: implement it
						break;
				}
			} else break;
		} else if(state == STATE_STRING_DATA) {
			//printf("STATE_STRING_DATA\n");
			if(input->hasBytes(currentLength)) {
				unsigned char *data = new unsigned char[currentLength];
				input->getBytes(data, currentLength);
				state = STATE_TYPE;
				string str((const char *)data, (size_t)currentLength);
				onString(str);
			} else break;
		} else if(state == STATE_ARRAY) {
			printf("STATE_ARRAY\n");
			if(input->hasBytes(currentLength)) {
				switch(currentLength) {
					case 1:
						onArray(input->getByte());
						state = STATE_TYPE;
						break;
					case 2:
						onArray(currentLength = input->getShort());
						state = STATE_TYPE;
						break;
					case 4:
						onArray(input->getInt());
						state = STATE_TYPE;
						break;
					case 8:
						//input->getLong();
						//currentLength = 123;
						state = STATE_TYPE;
						// TODO: implement it
						break;
				}
			} else break;
		} else if(state == STATE_MAP) {
			//printf("STATE_MAP\n");
			if(input->hasBytes(currentLength)) {
				switch(currentLength) {
					case 1:
						onMap(input->getByte());
						state = STATE_TYPE;
						break;
					case 2:
						onMap(currentLength = input->getShort());
						state = STATE_TYPE;
						break;
					case 4:
						onMap(input->getInt());
						state = STATE_TYPE;
						break;
					case 8:
						//input->getLong();
						//currentLength = 123;
						state = STATE_TYPE;
						// TODO: implement it
						break;
				}
			} else break;
		} else if(state == STATE_TAG) {
			//printf("STATE_TAG\n");
			if(input->hasBytes(currentLength)) {
				switch(currentLength) {
					case 1:
						onTag(input->getByte());
						state = STATE_TYPE;
						break;
					case 2:
						onTag(input->getShort());
						state = STATE_TYPE;
						break;
					case 4:
						onTag(input->getInt());
						state = STATE_TYPE;
						break;
					case 8:
						input->getLong();
						state = STATE_TYPE;
						// TODO: implement it
						break;
				}
			} else break;
		} else if(state == STATE_SPECIAL) {
			//printf("STATE_SPECIAL\n");
			if(input->hasBytes(currentLength)) {
				switch(currentLength) {
					case 1:
						onSpecial(input->getByte());
						state = STATE_TYPE;
						break;
					case 2:
						onSpecial(input->getShort());
						state = STATE_TYPE;
						break;
					case 4:
						// TODO: for integers > 2^31
						onSpecial(input->getInt());
						state = STATE_TYPE;
						break;
					case 8:
						input->getLong();
						state = STATE_TYPE;
						// TODO: implement it
						break;
				}
			} else break;
		} else {
			printf("UNKNOWN STATE\n");
		}
	}
}

CborOutput::CborOutput(int capacity) {
	this->capacity = capacity;
	this->buffer = new unsigned char[capacity];
	this->offset = 0;
}

CborOutput::~CborOutput() {
	delete buffer;
}

void CborOutput::putByte(unsigned char value) {
	fprintf(stderr, "put byte %d, current offset %d\n", value, offset);
	if(offset < capacity) {
		buffer[offset++] = value;
	} else {
		fprintf(stderr, "%s\n", "buffer overflow error");
	}
}

void CborOutput::putBytes(const unsigned char *data, int size) {
	fprintf(stderr, "put bytes with size %d, current offset %d\n", size, offset);
	if(offset + size - 1 < capacity) {
		memcpy(buffer + offset, data, size);
		offset += size;
	} else {
		fprintf(stderr, "%s\n", "buffer overflow error");	
	}
}

CborWriter::CborWriter(CborOutput &output) {
	this->output = &output;
}

CborWriter::~CborWriter() {

}

unsigned char *CborOutput::getData() {
	return buffer;
}

int CborOutput::getSize() {
	return offset;
}

void CborWriter::writeTypeAndValue(int majorType, unsigned int value) {
	majorType <<= 5;
	if(value < 24) {
		output->putByte(majorType | value);
	} else if(value < 256) {
		output->putByte(majorType | 24);
		output->putByte(value);
	} else if(value < 65536) {
		output->putByte(majorType | 25);
		output->putByte(value >> 8);
		output->putByte(value);
	} else {
		output->putByte(majorType | 26);
		output->putByte(value >> 24);
		output->putByte(value >> 16);
		output->putByte(value >> 8);
		output->putByte(value);
	}
}

void CborWriter::writeTypeAndValue(int majorType, unsigned long long value) {
	majorType <<= 5;
	if(value < 24ULL) {
		output->putByte(majorType | value);
	} else if(value < 256ULL) {
		output->putByte(majorType | 24);
		output->putByte(value);
	} else if(value < 65536ULL) {
		output->putByte(majorType | 25);
		output->putByte(value >> 8);
	} else if(value < 4294967296ULL) {
		output->putByte(majorType | 26);
		output->putByte(value >> 24);
		output->putByte(value >> 16);
		output->putByte(value >> 8);
		output->putByte(value);
	} else {
		output->putByte(majorType | 27);
		output->putByte(value >> 56);
		output->putByte(value >> 48);
		output->putByte(value >> 40);
		output->putByte(value >> 32);
		output->putByte(value >> 24);
		output->putByte(value >> 16);
		output->putByte(value >> 8);
		output->putByte(value);
	}
}

void CborWriter::writeInt(unsigned int value) {
	writeTypeAndValue(0, value);
}

void CborWriter::writeInt(unsigned long long value) {
	writeTypeAndValue(0, value);
}

void CborWriter::writeInt(long long value) {
	if(value < 0) {
		writeTypeAndValue(1, (unsigned long long) -value);
	} else {
		writeTypeAndValue(0, (unsigned long long) value);
	}
}

void CborWriter::writeInt(int value) {
	if(value < 0) {
		writeTypeAndValue(1, (unsigned int) -value);
	} else {
		writeTypeAndValue(0, (unsigned int) value);
	}
}

void CborWriter::writeBytes(const unsigned char *data, unsigned int size) {
	writeTypeAndValue(2, size);
	output->putBytes(data, size);
}

void CborWriter::writeString(const char *data, unsigned int size) {
	writeTypeAndValue(3, size);
	output->putBytes((const unsigned char *)data, size);
}

void CborWriter::writeString(const string str) {
	writeTypeAndValue(3, (unsigned int)str.size());
	output->putBytes((const unsigned char *)str.c_str(), str.size());
}


void CborWriter::writeArray(int size) {
	writeTypeAndValue(4, (unsigned int)size);
}

void CborWriter::writeMap(int size) {
	writeTypeAndValue(5, (unsigned int)size);
}

void CborWriter::writeTag(const unsigned int tag) {
	writeTypeAndValue(6, tag);
}

void CborWriter::writeSpecial(int special) {
	writeTypeAndValue(7, (unsigned int)special);
}

// TEST HANDLERS

void CborReader::onInteger(int value) {
	printf("integer: %d\n", value);
}

void CborReader::onBytes(unsigned char *data, int size) {
	printf("bytes with size: %d", size);
}

void CborReader::onString(string &str) {
	printf("string: '%.*s'\n", str.size(), str.c_str());
}

void CborReader::onArray(int size) {
	printf("array: %d\n", size);
}

void CborReader::onMap(int size) {
	printf("map: %d\n", size);
}

void CborReader::onTag(unsigned int tag) {
	printf("tag: %d\n", tag);
}

void CborReader::onSpecial(int code) {
	printf("special: %d\n", code);
}

void CborReader::onError(const char *error) {
	printf("error: %s\n", error);
}

// TEST HANDLERTE

void writeTest() {
	CborOutput output(4096);
	CborWriter writer(output);

	writer.writeArray(3);
	writer.writeInt(123);
	writer.writeInt(12);
	writer.writeString("hello", 5);

	//fwrite(output.getData(), 1, output.getSize(), stdout);

	unsigned char *data = output.getData();
	int size = output.getSize();

	CborInput input(data, size);
	CborReader reader(input);
	reader.run();
}

/*
int main(int argc, char **argv) {
	writeTest();
	return 0;
}
*/
