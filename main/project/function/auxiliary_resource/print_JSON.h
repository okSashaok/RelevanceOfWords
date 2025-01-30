#pragma once
class PrintJSON{
	private:
	std::string& GET;
	unsigned char layer = 0;
	bool pair = 0;
	void tab();
	void insert();
	public:
	PrintJSON(std::string& operGET);
	void operator()(const char* kay);
	void operator()(const char* kay, const char* comment);
	void operator()(const char* kay, float comment, unsigned char accuracy);
	void operator--();
	void operator--(int);
};