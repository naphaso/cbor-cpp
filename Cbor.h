#ifndef CBOR_H
#define CBOR_H


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

class CborReader {
public:
	CborReader(CborInput &input);
	~CborReader();
	void run();
protected:
	virtual void onInteger(int value);
	virtual void onBytes(unsigned char *data, int size);
	virtual void onString(unsigned char *data, int size);
	virtual void onArray(int size);
	virtual void onMap(int size);
	virtual void onTag(int tag);
	virtual void onSpecial(int code);
	virtual void onError(const char *error);
private:
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
	void writeBytes(const unsigned char *data, int size);
	void writeString(const char *data, int size);
	void writeArray(int size);
	void writeMap(int size);
	void writeTag(int tag);
	void writeSpecial(int special);
private:
	void writeTypeAndValue(int majorType, int value);
	CborOutput *output;
};

#endif
