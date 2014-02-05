#ifndef CBOR_H
#define CBOR_H

#include <string>

class CborInput {
public:
	CborInput(void *data, int size);
	~CborInput();

	bool hasBytes(int count);
	unsigned char getByte();
	unsigned short getShort();
	unsigned int getInt();
	unsigned long long getLong();
	void getBytes(void *to, int count);
private:
	unsigned char *data;
	int size;
	int offset;
};

typedef enum {
	STATE_TYPE,
	STATE_PINT,	
    STATE_NINT,
    STATE_BYTES_SIZE,
    STATE_BYTES_DATA,
    STATE_STRING_SIZE,
    STATE_STRING_DATA,
    STATE_ARRAY,
    STATE_MAP,
    STATE_TAG,
    STATE_SPECIAL
} CborReaderState;



class CborListener {
public:
	virtual void OnInteger(int value) = 0;
	virtual void OnBytes(unsigned char *data, int size) = 0;
	virtual void OnString(std::string &str) = 0;
	virtual void OnArray(int size) = 0;
	virtual void OnMap(int size) = 0;
	virtual void OnTag(unsigned int tag) = 0;
	virtual void OnSpecial(int code) = 0;
	virtual void OnError(const char *error) = 0;
};

class CborDebugListener : public CborListener {
public:
	virtual void OnInteger(int value);
	virtual void OnBytes(unsigned char *data, int size);
	virtual void OnString(std::string &str);
	virtual void OnArray(int size);
	virtual void OnMap(int size);
	virtual void OnTag(unsigned int tag);
	virtual void OnSpecial(int code);
	virtual void OnError(const char *error);
};

class CborReader {
public:
	CborReader(CborInput &input);
	CborReader(CborInput &input, CborListener &listener);
	~CborReader();
	void Run();
	void SetListener(CborListener &listener);
private:
	CborListener *listener;
	CborInput *input;
	CborReaderState state;
	int currentLength;
};


class CborOutput {
public:
	CborOutput(int capacity);
	~CborOutput();
	unsigned char *getData();
	int getSize();

	void putByte(unsigned char value);
	void putBytes(const unsigned char *data, int size);
private:
	unsigned char *buffer;
	int capacity;
	int offset;
};


class CborWriter {
public:
	CborWriter(CborOutput &output);
	~CborWriter();

	void writeInt(int value);
	void writeInt(long long value);
	void writeInt(unsigned int value);
	void writeInt(unsigned long long value);
	void writeBytes(const unsigned char *data, unsigned int size);
	void writeString(const char *data, unsigned int size);
	void writeString(const std::string str);
	void writeArray(int size);
	void writeMap(int size);
	void writeTag(const unsigned int tag);
	void writeSpecial(int special);
private:
	void writeTypeAndValue(int majorType, unsigned int value);
	void writeTypeAndValue(int majorType, unsigned long long value);
	CborOutput *output;
};

class CborSerializable {
public:
	virtual void Serialize(CborWriter &writer) = 0;
};

#endif
