#include "TestEngine.h"

int TestEngine::counter = 0;

TestEngine::TestEngine(int argc, std::string argv) :
	directory_path(argv)
{
	try {
		file_path = boost::filesystem::path(argv);
		if ( !boost::filesystem::exists(file_path) )
			std::cout << "Error processing command line input:" << std::endl <<
			"The file: " << argv[0] << " does not exist" << std::endl;
		else if (!boost::filesystem::is_directory(file_path) )
			std::cout << "Error processing command line input: expected a directory name." << std::endl;
		else
		{
			std::copy(boost::filesystem::directory_iterator(file_path),
				boost::filesystem::directory_iterator(), // directory_iterator::value_type
				std::ostream_iterator<boost::filesystem::directory_entry>(std::cout, "\n"));
		}
	}
	catch (boost::filesystem::filesystem_error& error)
	{
		std::cout << "Error accessing file system from command line arguments." << std::endl;
		std::cout << error.what();
	}
}

TestEngine::TestEngine(void)
{
}


TestEngine::~TestEngine(void)
{
}


std::string TestEngine::getNextPath()
{
	typedef std::vector<boost::filesystem::path> vec;             // store paths,
	vec v;                                // so we can sort them later

	try
	{
		file_path = boost::filesystem::path(directory_path);
		copy(boost::filesystem::directory_iterator(file_path), boost::filesystem::directory_iterator(), back_inserter(v));

		sort(v.begin(), v.end());             // sort, since directory iteration
		// is not ordered on some file systems
		int k = 0;
		for (int j = 0; j < 2; j++)
		{
			for (vec::const_iterator it (v.begin()); it != v.end(); ++it)
			{
				if (counter == k++)
				{
					counter++;
					return it->string();
				}
				std::cout << "   " << *it << '\n';
			}
			k = 0;
			counter = 0;
		}
	}
	catch (boost::filesystem::filesystem_error& error)
	{
		std::cout << "Error accessing file system from command line arguments." << std::endl;
		std::cout << error.what();
	}
	return "Error, fix me";
}

void TestEngine::parseDataFromDirectory(ResourceHandler* data_store)
{
	typedef std::vector<boost::filesystem::path> vec;             // store paths,
	vec v;                                // so we can sort them later

	try
	{
		file_path = boost::filesystem::path(directory_path);
		copy(boost::filesystem::directory_iterator(file_path), boost::filesystem::directory_iterator(), back_inserter(v));

		sort(v.begin(), v.end());             // sort, since directory iteration
		// is not ordered on some file systems
		for (vec::const_iterator it (v.begin()); it != v.end(); ++it)
		{
			if (strcmp(it->string().c_str(), "Meshes") == 0)
			{
			}
			if (strcmp(it->string().c_str(), "ShaderPrograms") == 0)
			{
			}
			if (strcmp(it->string().c_str(), "Textures") == 0)
			{
			}
			if (strcmp(it->string().c_str(), "Meshes") == 0)
			{
			}
		}
	}
	catch (boost::filesystem::filesystem_error& error)
	{
		std::cout << "Error accessing file system from command line arguments." << std::endl;
		std::cout << error.what();
	}
}