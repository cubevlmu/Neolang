#include "Serializer.hpp"

#include <filesystem>

namespace neo {

    ISGSerializer::ISGSerializer(const char* path)
        : m_stream {}
    {
        if (!std::filesystem::exists(path)) {
            LogError("[ISGSerializer] File not found -> ", path);
            FLIB_ASSERT(false);
        }

        m_stream.open(path, std::ios::in | std::ios::binary);
        if (!m_stream.is_open()) {
            LogError("[ISGSerializer] File can't be read -> ", path);
            FLIB_ASSERT(false);
        }
    }


    ISGSerializer::~ISGSerializer() {
        m_stream.close();
    }


    void* ISGSerializer::read(u64 size) {
        char* ptr = (char*)malloc(size * sizeof(char));
        m_stream.read(ptr, size);
        return ptr;
    }


    u64 ISGSerializer::read(void*& ptr, u64 size) {
        m_stream.read((char*)ptr, size);
        return size;
    }


    u64 ISGSerializer::getPos() {
        return m_stream.tellg();
    }

    void ISGSerializer::setPos(u64 pos) {
        m_stream.seekg(pos, std::ios::beg);
    }


    OSGSerializer::OSGSerializer(const char* path)
        : m_stream()
    {
        m_stream.open(path, std::ios::out | std::ios::binary);
        if (!m_stream.is_open()) {
            LogError("[OSGSerializer] File can't be write -> ", path);
            FLIB_ASSERT(false);
        }
    }


    OSGSerializer::~OSGSerializer() {
        m_stream.flush();
        m_stream.close();
    }


    void OSGSerializer::write(void* data, u64 size) {
        m_stream.write((char*)data, size);
    }


    void SGSerializer::write(const char* str) {
        i64 len = strlen(str);
        write(len);
        write((void*)str, sizeof(char) * len);
    }


    void SGSerializer::write(std::map<std::string, std::string> &map) {
      i64 len = map.size();
      for (auto &[k, v] : map) {
        write(k);
        write(v);
      }
    }

    void SGSerializer::write(std::unordered_map<std::string, std::string> &map) {
        i64 len = map.size();
        for(auto& [k,v] : map) {
            write(k);
            write(v);
        }
    }


    void SGSerializer::write(std::vector<std::string>& arr) {
        i64 size = arr.size();
        write(size);
        for (auto& item : arr) {
            write(item);
        }
    }


    void SGSerializer::write(bool v) {
        write((void*)&v, 1);
    }


    void SGSerializer::write(i8 v) {
        write((void*)&v, sizeof(i8));
    }


    void SGSerializer::write(i16 v) {
        write((void*)&v, sizeof(i16));
    }


    void SGSerializer::write(i32 v) {
        write((void*)&v, sizeof(i32));
    }


    void SGSerializer::write(i64 v) {
        write((void*)&v, sizeof(i64));
    }


    void SGSerializer::write(float v) {
        write((void*)&v, sizeof(float));
    }


    void SGSerializer::write(double v) {
        write((void*)&v, sizeof(double));
    }


    void SGSerializer::read(std::string& str) {
        i64 len{};
        read(len);
        std::vector<char> data(len);
        void* dptr = data.data();
        read(dptr, sizeof(char) * len);
        str = std::string{ data.data(), (u64)len };
    }


    void SGSerializer::read(std::map<std::string, std::string> &map) {
        i64 len {};
        read(len);
        for (i64 i = 0; i <= len; i++) {
            std::string k, v;
            read(k);
            read(v);
            map.insert({k,v});
        }
    }


    void SGSerializer::read(std::unordered_map<std::string, std::string> &map) {
        i64 len {};
        read(len);
        for (i64 i = 0; i <= len; i++) {
            std::string k, v;
            read(k);
            read(v);
            map.insert({k,v});
        }
    }


    void SGSerializer::read(std::vector<std::string>& arr) {
        i64 size{};
        read(size);
        for (i64 idx = 0; idx <= size; idx++) {
            std::string vv{};
            read(vv);
            arr.push_back(vv);
        }
    }


    void SGSerializer::read(bool& v) {
        void* vptr = &v;
        read(vptr, sizeof(bool));
    }


    void SGSerializer::read(i8& v) {
        void* vptr = &v;
        read(vptr, sizeof(i8));
    }


    void SGSerializer::read(i16& v) {
        void* vptr = &v;
        read(vptr, sizeof(i16));
    }


    void SGSerializer::read(i32& v) {
        void* vptr = &v;
        read(vptr, sizeof(i32));
    }


    void SGSerializer::read(i64& v) {
        void* vptr = &v;
        read(vptr, sizeof(i64));
    }


    void SGSerializer::read(float& v) {
        void* vptr = &v;
        read(vptr, sizeof(float));
    }


    void SGSerializer::read(double& v) {
        void* vptr = &v;
        read(vptr, sizeof(double));
    }

} // namespace lime