#include "LoginManager.h"
LoginManager* LoginManager::m_loginManagerInstace = nullptr;

// C'tors:

LoginManager::LoginManager(IDatabase* database)
{
	// Setting the database:
	m_database = database;

	// Opening the database:
	if (!m_database->open()) {
		throw std::exception("Could not open database");
	}
}

LoginManager* LoginManager::getInstance(IDatabase* database)
{
	if (m_loginManagerInstace == nullptr) {
		m_loginManagerInstace = new LoginManager(database);
	}

	return m_loginManagerInstace;
}


// D'tors:

LoginManager::~LoginManager()
{
	m_database = nullptr;
	delete m_loginManagerInstace;
}


// Methods:

/*
Logging-in to an account
Input : username - the username
		password - the password
Output: < None >
*/
void LoginManager::login(const string& username, const string& password)
{
	// Inits:
	int i = 0;

	// Condition: the user doesn't exist
	if (!m_database->doesUserExist(username)) {
		throw std::exception("User doesn't exist\n");
	}

	// Condition: the password doesn't match the username
	if (!m_database->doesPasswordMatch(username, password)) {
		throw std::exception("Password doesn't match\n");
	}

	for (i = 0; i < m_loggedUsers.size(); i++)
	{
		if (m_loggedUsers[i].getUsername() == username)
		{
			throw std::exception("User already logged-in\n");
		}
	}
	
	// Adding the logged user to the vector:
	m_loggedUsers.push_back(LoggedUser(username));
}

/*
Signing-up with a new account
Input : username - the username
		password - the password
		email	 - the email
Output: < None >
*/
void LoginManager::signup(const string& username, const string& password, const string& email,
	const string& address, const string& phoneNumber, const string& birthDate) const
{
	// Condition: the user already exists
	if (m_database->doesUserExist(username)) {
		throw std::exception("User already exists\n");
	}

	// Condition: password doesn't match regex
	std::regex passwordReg("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[@$!%*?&#])[A-Za-z\\d@$!%*?&#]{8,}$");
	if (!std::regex_match(password, passwordReg)) {
		throw std::exception("Password doesn't match requirements\n");
	}

	// Condition: email doesn't match regex
	std::regex emailReg("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
	if (!std::regex_match(email, emailReg)) {
		throw std::exception("Email doesn't match requirements\n");
	}
	
	// Condition: address doesn't match regex
	std::regex addressReg("([a-zA-Z]+)\\-([0-9]+)\\-([a-zA-Z]+)");
	if (!std::regex_match(address, addressReg)) {
		throw std::exception("Address doesn't match requirements\n");
	}

	// Condition: phoneNumber doesn't match regex
	std::regex phoneNumberReg1("0\\(?([0-9]{1})\\)?([ .-]?)([0-9]{3})\\2([0-9]{4})");
	std::regex phoneNumberReg2("0\\(?([0-9]{2})\\)?([ .-]?)([0-9]{3})\\2([0-9]{4})");
	if (!std::regex_match(phoneNumber, phoneNumberReg1) && !std::regex_match(phoneNumber, phoneNumberReg2)) {
		throw std::exception("Phone Number doesn't match requirements\n");
	}

	// Condition: birthDate doesn't match regex
	std::regex birthDateReg("([0-9]{2})\\.([0-9]{2})\\.([0-9]{4})");
	if (!std::regex_match(birthDate, birthDateReg)) {
		throw std::exception("Birth Date doesn't match requirements\n");
	}

	// Adding the new user to the DB:
	m_database->addNewUser(username, password, email, address, phoneNumber, birthDate);
}

/*
Logging-out of an account
Input : username - the username
Output: < None >
*/
void LoginManager::logout(const string& username)
{
	// Inits:
	bool exists = false;
	int i = 0;

	// Erasing the logged user from the vector:
	for (i = 0; i != m_loggedUsers.size() && !exists; i++)
	{
		// Condition: user found in the vector
		if (m_loggedUsers[i].getUsername() == username)
		{
			exists = true;
			m_loggedUsers.erase(m_loggedUsers.begin() + i);
		}
	}
	
	// Condition: user isn't logged in
	if (!exists) {
		throw std::exception("User is not logged in\n");
	}
}
