//
// Created by Suspense on 6/28/2023.
//

#ifndef AU_BYTEBUFFER_H
#define AU_BYTEBUFFER_H


namespace Audace {
	class ByteBuffer {
		char* buffer;
		int length;

	public:
		ByteBuffer(char* buffer, int length) : buffer(buffer), length(length) {}
		const char* getBuffer() {return buffer;}
		int getLength() {return length;}
	};

}

#endif //AU_BYTEBUFFER_H
