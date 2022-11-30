#include "val.h"

Value Value::operator+(const Value& op) const {
    Value retVal;
    switch(T) {
        case VINT:
            if(op.IsInt())
                retVal = Itemp + op.GetInt();
            else if(op.IsReal()) {
                retVal = (float) Itemp + op.GetReal();
            }
            break;
        case VREAL:
            if(op.IsReal())
                retVal = Rtemp + op.GetReal();
            else if(op.IsInt()) {
                retVal = Rtemp + (float) op.GetInt();
            }
            break;
        case VSTRING:
            if(op.IsString())
                retVal = Stemp + op.GetString();
        default:
            break;
    }
    return retVal;
}

Value Value::operator-(const Value& op) const {
    Value retVal;
    switch(T) {
        case VINT:
            if(op.IsInt())
                retVal = Itemp - op.GetInt();
            else if(op.IsReal()) {
                retVal = (float) Itemp - op.GetReal();
            }
            break;
        case VREAL:
            if(op.IsReal())
                retVal = Rtemp - op.GetReal();
            else if(op.IsInt()) {
                retVal = Rtemp - (float) op.GetInt();
            }
            break;
        default:
            break;
    }
    return retVal;
}

// multiply this by op
Value Value::operator*(const Value& op) const {
    Value retVal;
    switch(T) {
        case VINT:
            if(op.IsInt())
                retVal = Itemp * op.GetInt();
            else if(op.IsReal()) {
                retVal = (float) Itemp * op.GetReal();
            }
            break;
        case VREAL:
            if(op.IsReal())
                retVal = Rtemp * op.GetReal();
            else if(op.IsInt()) {
                retVal = Rtemp * (float) op.GetInt();
            }
            break;
        default:
            break;
    }
    return retVal;
}

// divide this by op
Value Value::operator/(const Value& op) const {
    Value retVal;
    switch(T) {
        case VINT:
            if(op.IsInt()) {
                if(op.GetInt() == 0) {
                    throw "RUNTIME ERROR: Divide-by-zero.";
                }
                retVal = Itemp / op.GetInt();
            } else if(op.IsReal()) {
                if(op.GetReal() == 0.0) {
                    throw "RUNTIME ERROR: Divide-by-zero.";
                }
                retVal = (float) Itemp / op.GetReal();
            }
            break;
        case VREAL:
            if(op.IsReal()) {
                if(op.GetReal() == 0.0) {
                    throw "RUNTIME ERROR: Divide-by-zero.";
                }
                retVal = Rtemp / op.GetReal();
            } else if(op.IsInt()) {
                if(op.GetInt() == 0) {
                    throw "RUNTIME ERROR: Divide-by-zero.";
                }
                retVal = Rtemp / (float) op.GetInt();
            }
            break;
        default:
            break;
    }
    return retVal;
}

Value Value::operator==(const Value& op) const {
    Value retVal;
    if(T == op.GetType()) {
        switch(T) {
            case VINT:
                retVal = Itemp == op.GetInt();
                break;
            case VBOOL:
                retVal = Btemp == op.GetBool();
                break;
            case VREAL:
                retVal = Rtemp == op.GetReal();
                break;
            case VSTRING:
                retVal = Stemp == op.GetString();
                break;
            default:
                break;
        }
    } else {
        switch(T) {
            case VINT:
                if(op.GetType() == VREAL) {
                    retVal = Itemp == op.GetReal();
                }
                break;
            case VREAL:
                if(op.GetType() == VINT) {
                    retVal = Rtemp == op.GetInt();
                }
                break;
            default:
                break;
        }
    }
    return retVal;
}

Value Value::operator>(const Value& op) const {
    Value retVal;
    if(T == op.GetType()) {
        switch(T) {
            case VINT:
                retVal = Itemp > op.GetInt();
                break;
            case VBOOL:
                retVal = Btemp > op.GetBool();
                break;
            case VREAL:
                retVal = Rtemp > op.GetReal();
                break;
            case VSTRING:
                retVal = Stemp > op.GetString();
                break;
            default:
                break;
        }
    } else {
        switch(T) {
            case VINT:
                if(op.GetType() == VREAL) {
                    retVal = Itemp > op.GetReal();
                }
                break;
            case VREAL:
                if(op.GetType() == VINT) {
                    retVal = Rtemp > op.GetInt();
                }
                break;
            default:
                break;
        }
    }
    return retVal;
}

Value Value::operator<(const Value& op) const {
    Value retVal;
    if(T == op.GetType()) {
        switch(T) {
            case VINT:
                retVal = Itemp < op.GetInt();
                break;
            case VBOOL:
                retVal = Btemp < op.GetBool();
                break;
            case VREAL:
                retVal = Rtemp < op.GetReal();
                break;
            case VSTRING:
                retVal = Stemp < op.GetString();
                break;
            default:
                break;
        }
    } else {
        switch(T) {
            case VINT:
                if(op.GetType() == VREAL) {
                    retVal = Itemp < op.GetReal();
                }
                break;
            case VREAL:
                if(op.GetType() == VINT) {
                    retVal = Rtemp < op.GetInt();
                }
                break;
            default:
                break;
        }
    }
    return retVal;
}