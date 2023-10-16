#ifndef OPTIONAL_H
#define OPTIONAL_H

template <typename T>
class Optional 
{
    private:
        bool hasValue;
        T value;

    public:
        Optional() : hasValue(false) {}

        Optional(const T& val) : hasValue(true), value(val) {}

        bool has_value() const {
            return hasValue;
        }

        T value_or(const T& defaultValue) const {
            return hasValue ? value : defaultValue;
        }

        constexpr T getValue() const {
            if (!hasValue) {
                throw std::runtime_error("Optional does not contain a value.");
            }
            return value;
        }
};

#endif
