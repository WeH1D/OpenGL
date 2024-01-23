#pragma once

class IndexBuffer 
{
	private:
		unsigned int _rendererID;
		unsigned int _count;
	public:
		IndexBuffer(const unsigned int* data ,unsigned int count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;
		unsigned int getCount() const;
};
