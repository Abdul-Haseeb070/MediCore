#ifndef PERSON_H
#define PERSON_H

class Person {
protected:
    int id;
    char name[100];
    char contact[20];
    char password[50];

public:
    /// @brief Construct a default person.
    Person();

    /// @brief Construct a person.
    /// @param pid Person identifier.
    /// @param pname Person name.
    /// @param pcontact Person contact.
    /// @param ppassword Person password.
    Person(int pid, const char* pname, const char* pcontact, const char* ppassword);

    /// @brief Virtual destructor for polymorphic cleanup.
    virtual ~Person();

    /// @brief Display role-specific details.
    /// @return Nothing.
    virtual void displayInfo() const = 0;

    /// @brief Fetch role label.
    /// @return Role as C string.
    virtual const char* getRole() const = 0;

    /// @brief Get person id.
    /// @return Integer id.
    int getId() const;

    /// @brief Get person name.
    /// @return Name as C string.
    const char* getName() const;

    /// @brief Get person contact.
    /// @return Contact as C string.
    const char* getContact() const;

    /// @brief Get person password.
    /// @return Password as C string.
    const char* getPassword() const;

    /// @brief Compare input password with stored password.
    /// @param input User-entered password.
    /// @return True on match.
    bool checkPassword(const char* input) const;
};

#endif
