#ifndef ADMIN_H
#define ADMIN_H

#include "Person.h"

class Admin : public Person {
public:
    /// @brief Construct a default admin.
    Admin();

    /// @brief Construct an admin.
    /// @param aid Admin id.
    /// @param aname Admin name.
    /// @param apassword Admin password.
    Admin(int aid, const char* aname, const char* apassword);

    /// @brief Default virtual destructor.
    virtual ~Admin();

    /// @brief Display admin summary.
    /// @return Nothing.
    virtual void displayInfo() const;

    /// @brief Role label.
    /// @return Admin text.
    virtual const char* getRole() const;
};

#endif
