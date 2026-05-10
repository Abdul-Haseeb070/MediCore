#ifndef STORAGE_H
#define STORAGE_H

#include <type_traits>
#include "Patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"

template <typename T>
class Storage {
    T data[100];
    int count;

public:
    /// @brief Construct empty storage.
    Storage() : count(0) {}

    /// @brief Add item to storage.
    /// @param item Item to copy.
    /// @return False when full.
    bool add(const T& item) {
        if (count >= 100) {
            return false;
        }
        data[count] = item;
        ++count;
        return true;
    }

    /// @brief Remove item by id.
    /// @param id Target id.
    /// @return True when removed.
    bool removeById(int id) {
        int index = -1;
        for (int i = 0; i < count; ++i) {
            if (matchesId(data[i], id)) {
                index = i;
                break;
            }
        }
        if (index == -1) {
            return false;
        }
        for (int j = index; j < count - 1; ++j) {
            data[j] = data[j + 1];
        }
        --count;
        return true;
    }

    /// @brief Find item by id.
    /// @param id Target id.
    /// @return Pointer to stored item or nullptr.
    T* findById(int id) {
        for (int i = 0; i < count; ++i) {
            if (matchesId(data[i], id)) {
                return &data[i];
            }
        }
        return 0;
    }

    /// @brief Get raw item array.
    /// @return Pointer to first item.
    T* getAll() {
        return &data[0];
    }

    /// @brief Get number of used items.
    /// @return Count value.
    int size() const {
        return count;
    }

private:
    /// @brief Internal id matcher per type.
    /// @param item Item instance.
    /// @param id Target id.
    /// @return True when item id matches.
    bool matchesId(const T& item, int id) {
        if constexpr (std::is_same<T, Patient>::value) {
            Patient probe(id, "", 0, "", "", "", 0.0f);
            return item == probe;
        } else if constexpr (std::is_same<T, Doctor>::value) {
            Doctor probe(id, "", "", "", "", 0.0f);
            return item == probe;
        } else if constexpr (std::is_same<T, Appointment>::value) {
            return item.getAppointmentId() == id;
        } else if constexpr (std::is_same<T, Bill>::value) {
            return item.getBillId() == id;
        } else if constexpr (std::is_same<T, Prescription>::value) {
            return item.getPrescriptionId() == id;
        }
        return false;
    }
};

#endif
