cbor-cpp
========

CBOR C++ serialization library

Just a simple SAX-like Concise Binary Object Representation (CBOR).

[http://tools.ietf.org/html/rfc7049](http://tools.ietf.org/html/rfc7049)

#### Examples

Writing:

```C++
  CborDynamicOutput output;
  CborWriter writer(output);

  writer.writeTag(123);
  writer.writeArray(3);
  writer.writeString("hello");
  writer.writeString("world");
  writer.writeInt(321);

  unsigned char *data = output.getData();
  int size = output.getSize();
```

Reading:

```C++
  class CborExampleListener : public CborListener {
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

  ...
  
  void CborExampleListener::OnInteger(int value) {
    printf("integer: %d\n", value);
  }
  
  void CborExampleListener::OnBytes(unsigned char *data, int size) {
    printf("bytes with size: %d", size);
  }
  
  void CborExampleListener::OnString(string &str) {
    printf("string: '%.*s'\n", (int)str.size(), str.c_str());
  }
  
  void CborExampleListener::OnArray(int size) {
    printf("array: %d\n", size);
  }
  
  void CborExampleListener::OnMap(int size) {
    printf("map: %d\n", size);
  }
  
  void CborExampleListener::OnTag(unsigned int tag) {
    printf("tag: %d\n", tag);
  }
  
  void CborExampleListener::OnSpecial(int code) {
    printf("special: %d\n", code);
  }
  
  void CborExampleListener::OnError(const char *error) {
    printf("error: %s\n", error);
  }
  
  ...
  CborInput input(data, size);
  CborReader reader(input);
  CborExampleListener listener;
  reader.SetListener(listener);
  reader.Run();
```
