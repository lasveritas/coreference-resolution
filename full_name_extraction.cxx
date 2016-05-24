#encoding "utf-8"
#GRAMMAR_KWSET ["places"]
#GRAMMAR_ROOT  root

NotPlace -> Word <h-reg1, GU=~["persn, famn, patrn, CONJ, PR, PART, INTJ"], kwset=~["stop","adjplace","misc", "places", "address"], ~quoted, ~h-reg2>;
NotPlace -> Word <h-reg1, GU=~["persn, famn, patrn, CONJ, PR, PART, INTJ"], kwset=~["stop","adjplace","misc", "places", "address"], ~quoted, wff="[А-Я][а-я]+-[А-Я][а-я]+">;

Nob_part -> Word <kwtype = "nob_part">;

ShortName -> Word<wff=/[А-Я]\./>;
UCName -> NotPlace<kwtype="names"> {outgram = "sg"};
UCName -> Word<kwtype="names", gram="persn">;

FirstName -> NotPlace<gram="persn"> | ShortName;
FamilyName -> NotPlace<gram="famn">;
FamilyName -> Word<wff="аль-[А-Я][а-я]+", kwtype="names">;
FamilyName -> Word<wff="эль-[А-Я][а-я]+", kwtype="names">;
FamilyName -> AnyWord<wff="[А-Я].+швили", kwtype="names">;
FamilyName -> AnyWord<wff="[А-Я].+дзе", kwtype="names">;
FamilyName -> AnyWord<wff="О'[А-я]+">;
FamilyName -> AnyWord<wff="О’[А-я]+">;
FamilyName -> FamilyName "(" FamilyName ")";
FamilyName -> Nob_part (Nob_part) FamilyName {weight=10};
FamilyName -> Nob_part (Nob_part) AnyWord<h-reg1> {weight=10};

PatrnName -> NotPlace<gram="patrn"> | ShortName;  

FamilyName2 -> FamilyName ;

FirstName_interp -> FirstName interp (+StrictName.Firstname);
FamilyName_interp -> FamilyName interp (+StrictName.Lastname);
FamilyName_interp -> NotPlace<gram="sg, famn"> interp (+StrictName.Lastname);
FamilyName_interp -> NotPlace<gram="pl, famn"> interp (+StrictName.Lastname::not_norm);

PatrnName_interp -> PatrnName interp (+StrictName.Patronymic);
UCName_Name_interp -> UCName interp (+StrictName.Firstname);
UCName_Surname_interp -> UCName interp (+StrictName.Lastname);  

FirstName_fam_interp -> FirstName<gram="sg"> interp (+StrictName.Lastname::not_norm);
FirstName_fam_interp -> FirstName<gram="pl"> interp (+StrictName.Lastname);

Unl -> UCName | FirstName | FamilyName;

FirstNamePaired -> FirstName FirstName<kwset=~["nob_part"]>;
FirstNamePaired -> FirstName UCName<kwset=~["nob_part"]>;
FNamePaired_interp -> FirstNamePaired interp (StrictName.Firstname);

EastFamilyName -> AnyWord<kwtype="east_sn", h-reg1>;
EastFirstName -> AnyWord<wff="[А-ЯЁ][а-яё]?[а-яё]?[а-яё]?", kwset=["names", "east_sn", "nob_part"]> AnyWord<wff="[А-ЯЁ][а-яё]?[а-яё]?[а-яё]?", kwset=["names", "east_sn"]>+;
FullName -> EastFamilyName<fw,kwtype=~"danger_east_sn" >  interp (StrictName.Lastname) AnyWord<wff="[А-Я][а-я]+", kwtype="names"> interp (StrictName.Firstname);
FullName -> EastFamilyName<~fw>  interp (StrictName.Lastname) AnyWord<wff="[А-Я][а-я]+", kwtype="names"> interp (StrictName.Firstname);
FullName -> EastFamilyName interp (StrictName.Lastname) EastFirstName interp (StrictName.Firstname) {weight=1.5};
FullName -> FirstName_interp EastFirstName interp (StrictName.Lastname);


FullNameOneWord -> FirstName_interp | FamilyName_interp | PatrnName_interp;
FullName -> FirstName_interp<rt> PatrnName_interp (FamilyName_interp);
FullName -> FirstName_interp<rt> PatrnName_interp UCName_Surname_interp;
FullName -> FamilyName_interp FirstName_interp<rt> (PatrnName_interp);
FullName -> FirstName_interp<rt> FamilyName_interp;
FullName -> FirstName_interp<rt> UCName_Surname_interp;
FullName -> FirstName_interp<rt> UCName_Surname_interp "(" FamilyName_interp ")";
FullName -> UCName_Name_interp<rt> UCName_Surname_interp;
FullName -> UCName_Name_interp FamilyName_interp;
FullName -> FirstName_interp<rt> FirstName_fam_interp;
FullName -> FNamePaired_interp<rt> UCName_Surname_interp;
FullName -> FNamePaired_interp<rt> FamilyName_interp;
FullName -> FNamePaired_interp<rt> FirstName_fam_interp ;

FullName -> ShortName interp (StrictName.Firstname) AnyWord<wff='[А-Я][а-я]+'> interp (StrictName.Lastname);

SpWord -> Word<wff="[А-ЯЁ][а-яё]+", gram="~persn", kwset=~["names", "nob_part", "names_not"]>;
SpWord2 -> SpWord<gram="~patrn">;

FullName -> FirstName_interp<rt> SpWord2 interp (StrictName.Lastname);

FullName -> FamilyName2 interp (StrictName.Firstname) NotPlace<kwtype="names"> interp (StrictName.Lastname);
FullNameOneWord0 -> NotPlace<kwtype="danger_east_sn", fw> Word;
FullNameOneWord -> NotPlace<kwtype="names"> interp (StrictName.Lastname);

FullName -> NotPlace<kwtype="names"> interp (StrictName.Firstname) FamilyName_interp;
FullName -> UCName_Name_interp (Unl interp (+StrictName.Lastname)) Unl interp (+StrictName.Lastname);

FullName -> FirstName_interp  AnyWord<h-reg1, gram="A"> interp (StrictName.Lastname);

FullName_interp -> FullName interp (StrictName.NameNotNorm::not_norm);
FullNameOneWord_interp -> FullNameOneWord interp (StrictName.NameNotNorm::not_norm);
FullNameOneWord_interp -> FullNameOneWord0;
  
FOAMale -> Word<kwtype="addressMale"> interp (StrictName.Gender="male");
FOAFemale -> Word<kwset="addressFemale"> interp (StrictName.Gender="female");
FOAFemale -> Word<kwtype="addressFemale_ledi"> interp (StrictName.Gender="female");
FOADual -> Word<kwtype="addressDual"> interp (StrictName.Gender="dual");
FOA_nointerp -> FOAMale | FOAFemale | FOADual;
FOA -> FOA_nointerp interp (StrictName.Address);

FOAName -> FOA FullName_interp;
FOAName -> FOA "и" FOA FullName_interp;

//descriptor
NotName -> Word<gram="~persn, ~famn, ~patrn", kwset=~['names']>;

GenGroup -> (Adj<gnc-agr[1]>) Noun<gram="gen", gnc-agr[1], l-reg>;
GenGroup -> GenGroup GenGroup+;

BareDescrNoun -> Noun<gram="sg, anim, ~persn", rt, kwset=~["address", "role", "not_descr"]>;

BareDescr -> (Adj<gnc-agr[0]>) BareDescrNoun< gnc-agr[0], l-reg, ~fw> (Adj<gnc-agr[1]>) (NotName<gram="gen", gnc-agr[1]>);
BareDescr -> (Adj<gnc-agr[0]>) BareDescrNoun< gnc-agr[0], l-reg, ~fw> NotName<h-reg>;
BareDescr -> BareDescr GenGroup;

BareDescr -> (Adj<gnc-agr[0]>) BareDescrNoun<gnc-agr[0], fw> (Adj<gnc-agr[1]>) (NotName<gram="gen", gnc-agr[1]>);
BareDescr -> (Adj<gnc-agr[0]>) BareDescrNoun<gnc-agr[0], fw> NotName<h-reg>;
BareDescr -> BareDescr GenGroup;

Descr -> BareDescr<gram="m, ~f"> interp (StrictName.Gender="male");
Descr -> BareDescr<gram="~m, f"> interp (StrictName.Gender="female");
Descr -> BareDescr<gram="m, f"> interp (StrictName.Gender="dual");
Descr -> BareDescr interp (StrictName.Gender="dual");
Descr -> Descr AnyWord<l-quoted> (AnyWord<~l-quoted>+) AnyWord<r-quoted>;
Descr -> Descr AnyWord<h-reg1, quoted>;


DescrName1 -> Descr FullName_interp;
DescrName1 -> Descr<gnc-agr[1]> "-" FullName_interp<gnc-agr[1]>;
DescrName1 -> Descr<gnc-agr[1]> Hyphen FullName_interp<gnc-agr[1]>;

DescrName -> DescrName1 Comma DescrName1;
DescrName -> DescrName1;
DescrName_interp -> DescrName interp (StrictName.Descriptor="true");

Role -> AnyWord<kwtype="role">;
FullName_interp -> Role FullNameOneWord_interp<gram="gen"> FullName_interp;
FullName_interp -> Role FullName_interp<gram="gen"> FullNameOneWord_interp;
FullName_interp -> Role FullName_interp<gram="gen"> FullName_interp;
FullName_interp -> Role FullName_interp<gram="gen">;

//False Examples
Loc -> Word<kwtype="adjplace">;
Org -> Word<kwtype="org">;
MiscTrue -> Loc Word<h-reg1>;
MiscTrue -> Org Word<h-reg1, gram="~persn">;
MiscTrue -> "в" Word<h-reg1>;
MiscTrue -> AnyWord<h-reg1, l-quoted> FullNameOneWord<r-quoted>;

root -> FullName_interp | FOAName | DescrName_interp | MiscTrue | FullNameOneWord_interp;

