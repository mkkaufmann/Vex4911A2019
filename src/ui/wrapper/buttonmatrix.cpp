#include "ui/wrapper/buttonmatrix.hpp"

ButtonMatrix ButtonMatrix::create(){
		return ButtonMatrix(lv_btnm_create(lv_scr_act(), NULL));
}

ButtonMatrix ButtonMatrix::create(const Object& iparent){
		return ButtonMatrix(lv_btnm_create(iparent.get(), NULL));
}

ButtonMatrix ButtonMatrix::create(const Object& iparent, const ButtonMatrix& icopy){
		return ButtonMatrix(lv_btnm_create(iparent.get(), icopy.get()));
}
