#pragma once

#include <common.hpp>
#include <fstream>
#include <map>
#include <unordered_map>

namespace neo {

    struct ISerializable;

    class SGSerializer {
    public:
        virtual void write(void* data, u64 size) = 0;
        virtual void* read(u64 size) = 0;
        virtual u64 read(void*& ptr, u64 size) = 0;
        virtual u64 getPos() = 0;
        virtual void setPos(u64 pos) = 0;

        void write(const char* str);
        void write(const std::string& str) {
            write(str.c_str());
        }
        void write(std::map<std::string, std::string>& map);
        void write(std::unordered_map<std::string, std::string>& map);
        void write(std::vector<std::string>& arr);
        void write(bool v);
        void write(i8 v);
        void write(i16 v);
        void write(i32 v);
        void write(i64 v);
        void write(float v);
        void write(double v);
        
        void read(std::string&);
        void read(std::map<std::string, std::string>& map);
        void read(std::unordered_map<std::string, std::string>& map);
        void read(std::vector<std::string>& arr);
        void read(bool&);
        void read(i8&);
        void read(i16&);
        void read(i32&);
        void read(i64&);
        void read(float&);
        void read(double&);
    };


    class ISGSerializer : public SGSerializer
    {
    public:
        ISGSerializer(const char* path);
        ~ISGSerializer();

        virtual void write(void* data, u64 size) override {
            (void)data; (void)size;
            LogError("[ISGSerializer] ReadOnly Serializer");
            FLIB_ASSERT(false);
            return;
        }
        virtual void* read(u64 size) override;
        virtual u64 read(void*& ptr, u64 size) override;
        virtual u64 getPos() override;
        virtual void setPos(u64 pos) override;

    private:
        std::ifstream m_stream;
    };


    class OSGSerializer : public SGSerializer
    {
    public:
        OSGSerializer(const char* path);
        ~OSGSerializer();

        virtual void write(void* data, u64 size) override;
        virtual void* read(u64 size) override {
            (void)size;
            LogError("[OSGSerializer] WriteOnly Serializer");
            FLIB_ASSERT(false);
            return nullptr;
        }
        virtual u64 read(void*& ptr, u64 size) override {
            (void)ptr;
            LogError("[OSGSerializer] WriteOnly Serializer");
            FLIB_ASSERT(false);
            return -1;
        }
        virtual u64 getPos() override {
            return m_stream.tellp();
        }
        virtual void setPos(u64 pos) override {
            m_stream.seekp(pos);
        }

    private:
        std::ofstream m_stream;
    };
}