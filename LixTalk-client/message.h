#ifndef LIXTALK_MESSAGE
#define LIXTALK_MESSAGE

#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

class message
{
public:
	message():allocator(doc_.GetAllocator()) {
		doc_.SetObject();
	}

	message(std::string msg) :allocator(doc_.GetAllocator()) {
		doc_.SetObject();
		doc_.Parse(msg.c_str());
	}

	void add(std::string key,int val) {
		rapidjson::Value n(key.c_str(), allocator);
		doc_.AddMember(n,val,allocator);
	}

	void add(std::string key, std::string val) {
		rapidjson::Value k(key.c_str(), allocator);
		rapidjson::Value v(val.c_str(), allocator);
		doc_.AddMember(k,v,allocator);
	}

	void add(std::string key, const QString& val) {
		rapidjson::Value k(key.c_str(), allocator);
		rapidjson::Value v(std::string(val.toUtf8().constData()), allocator);
		doc_.AddMember(k, v, allocator);
	}

	void clear() {
		doc_.SetObject();
	}

	std::string getString() {
		rapidjson::StringBuffer buffer;
		buffer.Clear();
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		doc_.Accept(writer);
		return buffer.GetString();
	}

	std::string getString(std::string key) {
		return doc_[key].GetString();
	}

	int getInt(std::string key) {
		return doc_[key].GetInt();
	}

	const rapidjson::Value& getArray(std::string key) {
		return doc_[key];
	}

private:
	rapidjson::Document doc_;
	rapidjson::MemoryPoolAllocator<>& allocator;
};

#endif
