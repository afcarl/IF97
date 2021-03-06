// if97_ths stub - Interfaces CoolProp IF97 function to Mathcad
//

// this code executes the user function if97_ths(h,s), which is a wrapper for
// the CoolProp-IF97 function, t_hsmass(h,s).
LRESULT  if97_THS(
    LPCOMPLEXSCALAR c,  // pointer to the result
    LPCCOMPLEXSCALAR a,
    LPCCOMPLEXSCALAR b) // pointer to the parameter received from Mathcad
{  
    // first check to make sure "a" and "b" have no imaginary component
    if ( a->imag != 0.0 )
        return MAKELRESULT(MUST_BE_REAL,1);
    if ( b->imag != 0.0 )
        return MAKELRESULT(MUST_BE_REAL,2);

    //otherwise, all is well, evaluate function
    try {
        c->real = IF97::T_hsmass(a->real,b->real);
    }
    catch (const std::out_of_range& e) { 
        if ((e.what()[0] == 'E') && (e.what()[3] == 'h'))       // e.what == "Enthalpy..."
            return MAKELRESULT(S_OUT_OF_RANGE,1);
        else if ((e.what()[0] == 'E') && (e.what()[3] == 'r'))  // e.what == "Entropy...")
            return MAKELRESULT(H_OUT_OF_RANGE,2);
        else if (e.what()[0] == 'U')                            // e.what == "Unable to determine region"
            return MAKELRESULT(REGION_NOT_FOUND,2);
        else                                                    // some other error
            return MAKELRESULT(NO_SOLUTION_FOUND,2);
    }
    catch (const std::logic_error&) {
        return MAKELRESULT(NO_SOLUTION_FOUND,1);
    }
    // normal return
    return 0;
}

FUNCTIONINFO    if97_ths = 
{
    "if97_ths",                      // name by which Mathcad will recognize the function
    "h,s",                           // if97_ths will be called as if97_ths(h,s)
    // description of if97_ths(h,s)
    "Obtains the temperature, T [K], as a function of mass enthalpy, h [J/kg], and mass entropy, s [J/kg-K].",
    (LPCFUNCTION)if97_THS,           // pointer to executable code
    COMPLEX_SCALAR,                  // the return type is a complex scalar
    2,                               // there are two input parameters
    { COMPLEX_SCALAR, COMPLEX_SCALAR }               //    that are both complex scalars
};