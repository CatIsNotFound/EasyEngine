#include "src/Core.h"

using namespace EasyEngine;
using namespace Components;
#include <vector>
#include <map>
#include <list>
#include <set>
#include <memory>

/**
 * 通用序列化和反序列化框架
 * 
 * 该框架支持：
 * 1. 基本数据类型（int, float, bool等）
 * 2. std::string类型
 * 3. 嵌套结构体和类
 * 4. 标准容器类型（std::vector, std::list, std::map等）
 * 5. 智能指针（std::shared_ptr, std::unique_ptr）
 * 
 * 核心思想是使用模板特化和SFINAE技术为不同类型提供专门的处理。
 */

// 基本的序列化接口定义
class Serializable {
public:
    virtual ~Serializable() = default;
    virtual void serialize(std::ofstream& file) const = 0;
    virtual void deserialize(std::ifstream& file) = 0;
};

// 类型特性类，用于判断类型是否为Serializable的派生类
template<typename T, typename = void>
struct is_serializable : std::false_type {};

template<typename T>
struct is_serializable<T, std::void_t<decltype(std::declval<T>().serialize(std::declval<std::ofstream&>()))>> 
    : std::conditional_t<std::is_base_of_v<Serializable, T>, std::true_type, std::false_type> {};

// 基本类型的序列化模板函数
template<typename T>
std::enable_if_t<!is_serializable<T>::value && !std::is_pointer_v<T>>
Serialize(std::ofstream& file, const T& value) {
    file.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

// 基本类型的反序列化模板函数
template<typename T>
std::enable_if_t<!is_serializable<T>::value && !std::is_pointer_v<T>>
Deserialize(std::ifstream& file, T& value) {
    file.read(reinterpret_cast<char*>(&value), sizeof(T));
}

// std::string类型的特化序列化函数
template<>
void Serialize<std::string>(std::ofstream& file, const std::string& value) {
    size_t len = value.size();
    file.write(reinterpret_cast<const char*>(&len), sizeof(len));
    file.write(value.c_str(), len);
}

// std::string类型的特化反序列化函数
template<>
void Deserialize<std::string>(std::ifstream& file, std::string& value) {
    size_t len;
    file.read(reinterpret_cast<char*>(&len), sizeof(len));
    value.resize(len);
    file.read(&value[0], len);
}

// 针对Serializable派生类的序列化函数
template<typename T>
std::enable_if_t<is_serializable<T>::value>
Serialize(std::ofstream& file, const T& value) {
    value.serialize(file);
}

// 针对Serializable派生类的反序列化函数
template<typename T>
std::enable_if_t<is_serializable<T>::value>
Deserialize(std::ifstream& file, T& value) {
    value.deserialize(file);
}

// std::vector容器的序列化
template<typename T>
void Serialize(std::ofstream& file, const std::vector<T>& vec) {
    size_t size = vec.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (const auto& item : vec) {
        Serialize(file, item);
    }
}

// std::vector容器的反序列化
template<typename T>
void Deserialize(std::ifstream& file, std::vector<T>& vec) {
    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    vec.resize(size);
    for (auto& item : vec) {
        Deserialize(file, item);
    }
}

// std::map容器的序列化
template<typename K, typename V>
void Serialize(std::ofstream& file, const std::map<K, V>& map) {
    size_t size = map.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (const auto& [key, value] : map) {
        Serialize(file, key);
        Serialize(file, value);
    }
}

// std::map容器的反序列化
template<typename K, typename V>
void Deserialize(std::ifstream& file, std::map<K, V>& map) {
    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    map.clear();
    for (size_t i = 0; i < size; ++i) {
        K key;
        V value;
        Deserialize(file, key);
        Deserialize(file, value);
        map[key] = value;
    }
}

// std::shared_ptr的序列化
template<typename T>
void Serialize(std::ofstream& file, const std::shared_ptr<T>& ptr) {
    bool isNull = (ptr == nullptr);
    file.write(reinterpret_cast<const char*>(&isNull), sizeof(isNull));
    if (!isNull) {
        Serialize(file, *ptr);
    }
}

// std::shared_ptr的反序列化
template<typename T>
void Deserialize(std::ifstream& file, std::shared_ptr<T>& ptr) {
    bool isNull;
    file.read(reinterpret_cast<char*>(&isNull), sizeof(isNull));
    if (isNull) {
        ptr = nullptr;
    } else {
        ptr = std::make_shared<T>();
        Deserialize(file, *ptr);
    }
}

// 测试用结构体1 - 继承自Serializable接口
struct Test : public Serializable {
    uint64_t id;
    std::string name;
    std::string data;
    
    // 构造函数
    Test() : id(0) {}
    Test(uint64_t _id, const std::string& _name, const std::string& _data)
        : id(_id), name(_name), data(_data) {}
    
    // 实现Serializable接口的序列化方法
    void serialize(std::ofstream& file) const override {
        Serialize(file, id);
        Serialize(file, name);
        Serialize(file, data);
    }
    
    // 实现Serializable接口的反序列化方法
    void deserialize(std::ifstream& file) override {
        Deserialize(file, id);
        Deserialize(file, name);
        Deserialize(file, data);
    }
};

// 测试用结构体2 - 同样继承自Serializable接口
struct Person : public Serializable {
    int age;
    float height;
    bool isStudent;
    std::string name;
    std::string address;
    
    // 构造函数
    Person() : age(0), height(0.0f), isStudent(false) {}
    Person(int _age, float _height, bool _isStudent, const std::string& _name, const std::string& _address)
        : age(_age), height(_height), isStudent(_isStudent), name(_name), address(_address) {}
    
    // 实现Serializable接口
    void serialize(std::ofstream& file) const override {
        Serialize(file, age);
        Serialize(file, height);
        Serialize(file, isStudent);
        Serialize(file, name);
        Serialize(file, address);
    }
    
    void deserialize(std::ifstream& file) override {
        Deserialize(file, age);
        Deserialize(file, height);
        Deserialize(file, isStudent);
        Deserialize(file, name);
        Deserialize(file, address);
    }
};

// 复杂数据类型示例1：嵌套结构体
struct Address {
    std::string street;
    std::string city;
    std::string country;
    int zipCode;
    
    Address() : zipCode(0) {}
    Address(const std::string& _street, const std::string& _city, const std::string& _country, int _zipCode)
        : street(_street), city(_city), country(_country), zipCode(_zipCode) {}
    
    // 自定义序列化和反序列化方法
    void serialize(std::ofstream& file) const {
        Serialize(file, street);
        Serialize(file, city);
        Serialize(file, country);
        Serialize(file, zipCode);
    }
    
    void deserialize(std::ifstream& file) {
        Deserialize(file, street);
        Deserialize(file, city);
        Deserialize(file, country);
        Deserialize(file, zipCode);
    }
};

// 复杂数据类型示例2：包含容器和智能指针的结构体
struct ComplexData : public Serializable {
    int id;
    std::vector<int> numbers;
    std::map<std::string, Person> personMap;
    std::shared_ptr<Test> testPtr;
    std::vector<Address> addresses;
    
    ComplexData() : id(0) {}
    ComplexData(int _id) : id(_id) {}
    
    // 实现Serializable接口
    void serialize(std::ofstream& file) const override {
        Serialize(file, id);
        Serialize(file, numbers);
        Serialize(file, personMap);
        Serialize(file, testPtr);
        Serialize(file, addresses);
    }
    
    void deserialize(std::ifstream& file) override {
        Deserialize(file, id);
        Deserialize(file, numbers);
        Deserialize(file, personMap);
        Deserialize(file, testPtr);
        Deserialize(file, addresses);
    }
};

// 用于打印ComplexData的辅助函数
auto printComplexData(const ComplexData& data) {
    fmt::println("ComplexData [ID: {}]", data.id);
    
    fmt::println("Numbers:");
    for (const auto& num : data.numbers) {
        fmt::print("{}", num);
        if (&num != &data.numbers.back()) fmt::print(", ");
    }
    fmt::println("");
    
    fmt::println("Person Map:");
    for (const auto& [key, person] : data.personMap) {
        fmt::println("  Key: {}", key);
        fmt::println("    Age: {}, Height: {}, IsStudent: {}, Name: {}, Address: {}", 
                     person.age, person.height, person.isStudent ? "true" : "false", 
                     person.name, person.address);
    }
    
    fmt::println("Test Pointer:");
    if (data.testPtr) {
        fmt::println("  ID: {}, Name: {}, Data: {}", 
                     data.testPtr->id, data.testPtr->name, data.testPtr->data);
    } else {
        fmt::println("  nullptr");
    }
    
    fmt::println("Addresses:");
    for (size_t i = 0; i < data.addresses.size(); ++i) {
        const auto& addr = data.addresses[i];
        fmt::println("  Address {}", i+1);
        fmt::println("    Street: {}", addr.street);
        fmt::println("    City: {}", addr.city);
        fmt::println("    Country: {}", addr.country);
        fmt::println("    Zip Code: {}", addr.zipCode);
    }
}

int main(int argc, char* argv[]) {
    FileSystem::setCurrentPath("D:/TestDir");
    
    if (argc > 1 && !strcmp(argv[1], "-w")) {
        Test t1(1, "ID1", "1298378912372819379128471289837913");
        Test t2(2, "ID2", "asjdkasjkdhaskdjhaskjdhaskdjhaskdjasd");
        FileSystem::mkFile("./Test.bin");
        
        // 使用函数式API和模板函数进行正确的序列化
        FileSystem::writeBinaryFile("./Test.bin", false, [&t1, &t2](std::ofstream& file){
            t1.serialize(file);
            t2.serialize(file);
        });
        
        fmt::println("Test data has been written successfully!");
    } else if (argc > 1 && !strcmp(argv[1], "-r")) {
        Test t1, t2;
        
        FileSystem::readBinaryFile("./Test.bin", [&t1, &t2](std::ifstream& file){
            t1.deserialize(file);
            t2.deserialize(file);
        });
        
        // 打印读取的内容
        fmt::println("[T1] ID: {}, name: {}, data: {}\n" 
                     "[T2] ID: {}, name: {}, data: {}", 
                     t1.id, t1.name, t1.data, t2.id, t2.name, t2.data);
    } else if (argc > 1 && !strcmp(argv[1], "-p")) {
        // 测试Person结构体的序列化和反序列化功能
        if (argc > 2 && !strcmp(argv[2], "-w")) {
            Person p1(25, 178.5f, true, "Alice", "123 Main St");
            Person p2(30, 185.0f, false, "Bob", "456 Elm Ave");
            FileSystem::mkFile("./Person.bin");
            
            // 复用相同的模板函数
            FileSystem::writeBinaryFile("./Person.bin", false, [&p1, &p2](std::ofstream& file){
                p1.serialize(file);
                p2.serialize(file);
            });
            
            fmt::println("Person data has been written successfully!");
        } else if (argc > 2 && !strcmp(argv[2], "-r")) {
            Person p1, p2;
            
            FileSystem::readBinaryFile("./Person.bin", [&p1, &p2](std::ifstream& file){
                p1.deserialize(file);
                p2.deserialize(file);
            });
            
            // 打印读取的内容
            fmt::println("[Person1] Age: {}, Height: {}, IsStudent: {}, Name: {}, Address: {}\n" 
                         "[Person2] Age: {}, Height: {}, IsStudent: {}, Name: {}, Address: {}", 
                         p1.age, p1.height, p1.isStudent ? "true" : "false", p1.name, p1.address, 
                         p2.age, p2.height, p2.isStudent ? "true" : "false", p2.name, p2.address);
        }
    } else if (argc > 1 && !strcmp(argv[1], "-c")) {
        // 测试复杂数据类型的序列化和反序列化功能
        if (argc > 2 && !strcmp(argv[2], "-w")) {
            // 创建一个复杂数据结构实例
            ComplexData data(1001);
            
            // 添加一些数值到vector
            for (int i = 1; i <= 5; ++i) {
                data.numbers.push_back(i * 10);
            }
            
            // 添加Person对象到map
            data.personMap["alice"] = Person(25, 178.5f, true, "Alice", "123 Main St");
            data.personMap["bob"] = Person(30, 185.0f, false, "Bob", "456 Elm Ave");
            
            // 创建并添加智能指针
            data.testPtr = std::make_shared<Test>(500, "ComplexTest", "This is test data inside a shared_ptr");
            
            // 添加地址信息
            data.addresses.push_back(Address("123 Main St", "New York", "USA", 10001));
            data.addresses.push_back(Address("456 Elm Ave", "London", "UK", 10002));
            
            FileSystem::mkFile("./ComplexData.bin");
            
            // 使用新的框架序列化复杂数据
            FileSystem::writeBinaryFile("./ComplexData.bin", false, [&data](std::ofstream& file){
                data.serialize(file);
            });
            
            fmt::println("Complex data has been written successfully!");
        } else if (argc > 2 && !strcmp(argv[2], "-r")) {
            ComplexData data;
            
            FileSystem::readBinaryFile("./ComplexData.bin", [&data](std::ifstream& file){
                data.deserialize(file);
            });
            
            // 打印复杂数据的内容
            printComplexData(data);
        }
    } else {
        fmt::println("Nothing to do! Try:\n" 
                     "-w: Write Test data\n" 
                     "-r: Read Test data\n" 
                     "-p -w: Write Person data\n" 
                     "-p -r: Read Person data\n" 
                     "-c -w: Write ComplexData\n" 
                     "-c -r: Read ComplexData");
    }
    
    return 0;
}