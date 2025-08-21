# Conventions

# Print
Start with a uppercased letter but does not end with a period.
```cpp
ns::printf("Could not get \"{}\" from table");
```

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
void my_awesome_command(ns::CommandContext* pCtx) {
    ...
}

void quit_command(ns::CommandContext* pCtx) {
    ...
}
```

**Commandline:**
```
my_awesome
quit
```