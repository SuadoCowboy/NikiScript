# Print
Start with a uppercased letter but does not end with a period.

- **Example:**
sci::printf("Could not get \"{}\" from table");

# Naming

## Functions
```cpp
uint8_t getUInt8FromBackRoom() {
    ...
}
```

## Variables
```cpp
std::string pluginDescription = "";
MyClass myObject{};
void* pMyPointer = nullptr;
```

## Commands
**Code:**
```cpp
void my_awesome_command(sci::SweatContext& ctx) {
    ...
}

void quit_command(sci::SweatContext& ctx) {
    ...
}
```

**Commandline:**
```
my_awesome
quit
```