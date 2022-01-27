#include <iostream>
#include "rapidjson/reader.h"  				//--download-from--:https://github.com/Tencent/rapidjson/blob/v1.1.0/include/rapidjson/reader.h
//#include "rapidjson/rapidjson.h"   		//--download-from--:https://github.com/Tencent/rapidjson/blob/v1.1.0/include/rapidjson/rapidjson.h
//#include "rapidjson/allocators.h"  		//--download-from--:https://github.com/Tencent/rapidjson/blob/v1.1.0/include/rapidjson/allocators.h
//#include "rapidjson/encodedstream.h"  	//--download-from--:https://github.com/Tencent/rapidjson/blob/v1.1.0/include/rapidjson/encodedstream.h
//#include "rapidjson/encodings.h"   		//--download-from--:https://github.com/Tencent/rapidjson/blob/v1.1.0/include/rapidjson/encodings.h
//#include "rapidjson/memorystream.h"   	//--download-from--:https://github.com/Tencent/rapidjson/blob/v1.1.0/include/rapidjson/memorystream.h
//#include "rapidjson/stream.h"   			//--download-from--:https://github.com/Tencent/rapidjson/blob/v1.1.0/include/rapidjson/stream.h
//#include "rapidjson/error/error.h"   		//--download-from--:https://github.com/Tencent/rapidjson/blob/v1.1.0/include/rapidjson/error/error.h
//#include "rapidjson/internal/biginteger.h"//--download-from--:https://github.com/Tencent/rapidjson/blob/v1.1.0/include/rapidjson/internal/biginteger.h
//#include "rapidjson/internal/diyfp.h"   	//--download-from--:https://github.com/Tencent/rapidjson/blob/v1.1.0/include/rapidjson/internal/diyfp.h
//#include "rapidjson/internal/ieee754.h"   //--download-from--:https://github.com/Tencent/rapidjson/blob/v1.1.0/include/rapidjson/internal/ieee754.h
//#include "rapidjson/internal/meta.h" 		//--download-from--:https://github.com/Tencent/rapidjson/blob/v1.1.0/include/rapidjson/internal/meta.h
//#include "rapidjson/internal/pow10.h"   	//--download-from--:https://github.com/Tencent/rapidjson/blob/v1.1.0/include/rapidjson/internal/pow10.h
//#include "rapidjson/internal/stack.h"  	//--download-from--:https://github.com/Tencent/rapidjson/blob/v1.1.0/include/rapidjson/internal/stack.h
//#include "rapidjson/internal/strtod.h"   	//--download-from--:https://github.com/Tencent/rapidjson/blob/v1.1.0/include/rapidjson/internal/strtod.h
//#include "rapidjson/internal/swap.h"   	//--download-from--:https://github.com/Tencent/rapidjson/blob/v1.1.0/include/rapidjson/internal/swap.h

using namespace rapidjson;
using namespace std;

struct MyHandler {
    bool Null() { cout << "Null()" << endl; return true; }
    bool Bool(bool b) { cout << "Bool(" << boolalpha << b << ")" << endl; return true; }
    bool Int(int i) { cout << "Int(" << i << ")" << endl; return true; }
    bool Uint(unsigned u) { cout << "Uint(" << u << ")" << endl; return true; }
    bool Int64(int64_t i) { cout << "Int64(" << i << ")" << endl; return true; }
    bool Uint64(uint64_t u) { cout << "Uint64(" << u << ")" << endl; return true; }
    bool Double(double d) { cout << "Double(" << d << ")" << endl; return true; }
    bool RawNumber(const char* str, SizeType length, bool copy) { 
        cout << "Number(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }
    bool String(const char* str, SizeType length, bool copy) { 
        cout << "String(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }
    bool StartObject() { cout << "StartObject()" << endl; return true; }
    bool Key(const char* str, SizeType length, bool copy) {
        cout << "Key(" << str << ", " << length << ", " << boolalpha << copy << ")" << endl;
        return true;
    }
    bool EndObject(SizeType memberCount) { cout << "EndObject(" << memberCount << ")" << endl; return true; }
    bool StartArray() { cout << "StartArray()" << endl; return true; }
    bool EndArray(SizeType elementCount) { cout << "EndArray(" << elementCount << ")" << endl; return true; }
};

int main() {
    const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";

    MyHandler handler;
    Reader reader;
    StringStream ss(json);
    reader.Parse(ss, handler);

    return 0;
}
