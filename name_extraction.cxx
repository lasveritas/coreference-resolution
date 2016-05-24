#encoding "utf-8"
#GRAMMAR_ROOT root 
#define ALL_CASES [nom,acc,dat,ins,loc,gen]

//extra
Conj -> "и" | Comma;
OrgBegin -> 'ооо' | 'зао' | 'оао';

//name parts
BaseWord -> Word<gram="S, sg", kwset=~['names_not'], wff="[А-ЯЁ][а-яё]+", ~h-reg2, ~quoted, ~l-quoted>;
BaseWord -> Word<gram="S, sg", kwset=~['names_not'], wff="[А-ЯЁ][а-яё]+-[А-ЯЁ][а-яё]+", ~h-reg2, ~quoted, ~l-quoted>;


FirstName -> BaseWord<gram="persn">;
FirstName -> BaseWord<gram ="persn, nom,~pl,~gen,~f,m">  interp (RawName.StrongNom=true);
Nob_part -> Word<kwtype="nob_part">;
Surname -> (Nob_part) BaseWord<gram="famn">;
SpSurname -> AnyWord<wff="[А-Я].+швили", ~dict>;
SpSurname -> AnyWord<wff="[А-Я].+дзе", ~dict>;
SpSurname -> AnyWord<wff="аль-[А-Я].+", ~dict>;
SpSurname -> AnyWord<wff="эль-[А-Я][а-я]+", ~dict>;
SpSurname -> AnyWord<kwtype="nob_part", cut> AnyWord<wff="[А-Я][а-я]+", ~dict>;

Patronymic -> BaseWord<gram="patrn">;

Name -> FirstName | Surname | Patronymic | SpSurname interp (RawName.Name; RawName.Name_not_norm::not_norm; RawName.Pers_check=true);
FOA -> Word<kwtype="address", gram = "sg">;
FOA_interp -> FOA<gram="nom", GU=~${ALL_CASES}> interp (RawName.StrongNom=true);
Anim -> Word<l-reg, gram = "S, sg, anim, ~persn, ~patrn, ~famn ">; 
NotAnim -> Word<l-reg, gram = "S, sg, inan, ~persn, ~patrn, ~famn, ~anim ">; 

UnknownWord0 -> AnyWord<~dict, ~quoted, ~fw>;
UnknownWord1 -> UnknownWord0<~l-quoted>;
UnknownWord -> UnknownWord1<~r-quoted>;
UCName -> UnknownWord<wff="[А-ЯЁ][а-яё]+">;
UCName -> UnknownWord<wff="[А-ЯЁ][а-яё]+-[А-ЯЁ][а-яё]+">;
DictName0 -> BaseWord<gram="~geo">;
DictName1 -> DictName0<gram="~famn">;
DictName -> DictName1<gram="~persn">;

NotNounName0 -> Word<~fw, h-reg1, ~h-reg2, ~quoted, wff='[А-Я][а-я]+', gram="~persn", kwset=~["nob_part"]>;
NotNounName1 -> NotNounName0<~l-quoted, gram="~famn">;
NotNounName2 -> NotNounName1<~r-quoted, gram="~patrn">;
NotNounName -> NotNounName2<gram="~geo"> interp (RawName.Name::not_norm; RawName.Name_not_norm::not_norm);
NotNounName -> NotNounName2 interp (RawName.Name::not_norm; RawName.Name_not_norm::not_norm);

NotNounNameBad -> NotNounName2<gram="~geo, A", gnc-agr[1]> interp (RawName.Name::not_norm; RawName.Name_not_norm::not_norm) Noun<gnc-agr[1], cut>;
NotNounNameBad ->  Word<wff="[А-Я][а-я]+ий", GU=&["~persn"], nc-agr[1]> interp (RawName.Name::not_norm; RawName.Name_not_norm::not_norm) (Adj<nc-agr[1], cut>+) Noun<nc-agr[1], cut, rt>;
NotNounNameBad -> Word<wff="[А-Я][а-я]+ая", GU=&["~persn"], nc-agr[1]> interp (RawName.Name::not_norm; RawName.Name_not_norm::not_norm) (Adj<nc-agr[1], cut>+) Noun<nc-agr[1], cut, rt>;
NotNounNameBad -> Word<wff="[А-Я][а-я]+ой", GU=&["~persn"], nc-agr[1]> interp (RawName.Name::not_norm; RawName.Name_not_norm::not_norm) (Adj<nc-agr[1], cut>+) Noun<nc-agr[1], cut, rt>;
NotNounNameBad -> Word<wff="[А-Я][а-я]+ый", GU=&["~persn"], nc-agr[1]> interp (RawName.Name::not_norm; RawName.Name_not_norm::not_norm) (Adj<nc-agr[1], cut>+) Noun<nc-agr[1], cut, rt>;
NotNounNameBad -> Word<wff="[А-Я][а-я]+ого", GU=&["~persn"], nc-agr[1]> interp (RawName.Name::not_norm; RawName.Name_not_norm::not_norm) (Adj<nc-agr[1], cut>+) Noun<nc-agr[1], cut, rt>;

NotNounNameBad ->  Word<wff="[А-Я][а-я]+ий",  nc-agr[1], ~dict> interp (RawName.Name::not_norm; RawName.Name_not_norm::not_norm) (Adj<nc-agr[1], cut>+) Noun<nc-agr[1], cut, rt>;
NotNounNameBad -> Word<wff="[А-Я][а-я]+ая", nc-agr[1], ~dict> interp (RawName.Name::not_norm; RawName.Name_not_norm::not_norm) (Adj<nc-agr[1], cut>+) Noun<nc-agr[1], cut, rt>;
NotNounNameBad -> Word<wff="[А-Я][а-я]+ой",  nc-agr[1], ~dict> interp (RawName.Name::not_norm; RawName.Name_not_norm::not_norm) (Adj<nc-agr[1], cut>+) Noun<nc-agr[1], cut, rt>;
NotNounNameBad -> Word<wff="[А-Я][а-я]+ый",  nc-agr[1], ~dict> interp (RawName.Name::not_norm; RawName.Name_not_norm::not_norm) (Adj<nc-agr[1], cut>+) Noun<nc-agr[1], cut, rt>;
NotNounNameBad -> Word<wff="[А-Я][а-я]+ого", nc-agr[1], ~dict> interp (RawName.Name::not_norm; RawName.Name_not_norm::not_norm) (Adj<nc-agr[1], cut>+) Noun<nc-agr[1], cut, rt>;
NotNounNameBad -> AnyWord<wff="[А-Я][а-я]+ой", ~dict> interp (RawName.Name::not_norm; RawName.Name_not_norm::not_norm) (Adj<cut, gnc-agr[1]>+) Noun<cut, rt, gnc-agr[1]>;



DictOrNot -> DictName | UCName;

//miscellaneous context
AdjPlace -> Word<kwtype="adjplace">;
AdjNESW -> "южный" | "западный" | "северный" | "восточный";
LocNameTrue -> AnyWord<wff="[А-Я][а-я]+полис">;
LocNameTrue -> DictOrNot AdjPlace<cut>;
LocNameTrue -> AdjPlace<cut> DictOrNot;
LocNameTrue -> AdjPlace<cut> DictName UCName;
LocNameTrue -> AdjPlace<cut> Word<h-reg1> UCName;
LocNameTrue -> AdjNESW<gnc-agr[1]> Word<h-reg1, gnc-agr[1]>;
LocNameTrue -> AdjNESW UCName;
LocNameFalse -> Word<h-reg1, gnc-agr[1]> AdjPlace<gnc-agr[1]> DictOrNot;
LocNameTrue -> AnyWord<wff="Нью-[А-Я][а-я]+">;
LocNameTrue -> AnyWord<wff="Сан-[А-Я][а-я]+">;


LocNameTrue -> "в"<cut> LocNameTrue;
LocNameTrue -> "в"<cut> UCName;
LocNameTrue -> "в"<cut> "сан" AnyWord<h-reg1>;
LocNameTrue -> "в"<cut> "аль" AnyWord<h-reg1>;
LocNameTrue -> "на"<cut> LocNameTrue;
LocNameTrue -> "на"<cut> UCName;

LocTrue_interp -> LocNameTrue interp (RawName.Name; RawName.Name_not_norm::not_norm;RawName.Misc_check=true);
LocTrue_interp -> LocNameFalse;

OrgTrue -> AnyWord<wff="[А-Я][а-яё]+новост?[а-яё]+">;
OrgTrue -> AnyWord<wff="Новост?[а-яё]+">;
OrgTrue -> AnyWord<wff="[А-Я][а-яё]+комисс?[а-яё]+">;
OrgTrue -> OrgBegin<cut> UCName;
OrgTrue -> Word<kwtype="org", cut> UCName;
OrgTrue -> UCName Word<kwtype="org", cut>;
OrgTrue -> "представитель"<cut> DictOrNot;
OrgTrue -> Anim<cut> DictName<gram="gen"> FirstName<cut> Surname<cut>;
OrgTrue -> Anim<cut> UCName FirstName Surname<cut>;
OrgTrue -> Anim<cut> DictName<gram="gen"> Surname<cut> FirstName<cut>;
OrgTrue -> Anim<cut> UCName Surname<cut> FirstName<cut>;
OrgTrue -> Word<kwtype="org", cut, nc-agr[1]> Word<h-reg1, nc-agr[1], gram="~persn">;

BareOrg_interp -> DictOrNot interp (RawName.Name; RawName.Name_not_norm::not_norm;RawName.Misc_check=true);
BareOrg_interp -> NotNounName interp (RawName.Name; RawName.Name_not_norm::not_norm;RawName.Misc_check=true);
OrgTrue_interp -> OrgTrue interp (RawName.Name; RawName.Name_not_norm::not_norm;RawName.Misc_check=true);

//person context
DictName_interp -> BaseWord<GU=~[geo, persn, famn, patrn]> interp (RawName.Name; RawName.Name_not_norm::not_norm);
UnknownName_interp -> UCName interp (RawName.Name::not_norm; RawName.Name_not_norm::not_norm);

PersnName_interp -> UnknownName_interp interp (RawName.Pers_check=true);
NotNounName_interp -> NotNounName interp (RawName.Pers_check=true);
NotNounName_interp -> NotNounNameBad interp (RawName.Misc_check=true);

PersNameTrue -> NotNounName_interp Name;
PersNameTrue -> Name NotNounName_interp;
PersNameTrue -> Name NotNounName_interp NotNounName_interp;
PersNameTrue -> Name NotNounName_interp UnknownName_interp;
PersNameTrue -> FOA NotNounName_interp;

PersNameTrue -> Name PersnName_interp+;
PersNameTrue -> Anim PersnName_interp PersnName_interp;
PersNameTrue -> Anim Name PersnName_interp;
PersNameTrue -> Anim Word<h-reg2> Name PersnName_interp;
PersNameTrue -> Anim Word<h-reg2> PersnName_interp PersnName_interp;

PersNameTrue -> Anim LocTrue_interp Name PersnName_interp;
PersNameTrue -> Anim Word<gram="geo"> Name PersnName_interp;
PersNameTrue -> Anim OrgTrue_interp Name PersnName_interp;
PersNameTrue -> Anim BareOrg_interp Name PersnName_interp;

PersNameTrue -> Anim LocTrue_interp PersnName_interp PersnName_interp;
PersNameTrue -> Anim Word<gram="geo"> PersnName_interp PersnName_interp;
PersNameTrue -> Anim OrgTrue_interp PersnName_interp PersnName_interp;
PersNameTrue -> Anim BareOrg_interp PersnName_interp PersnName_interp;

PersNameTrue -> Anim LocTrue_interp Name Name;
PersNameTrue -> Anim Word<gram="geo"> Name Name;
PersNameTrue -> Anim OrgTrue_interp Name Name;
PersNameTrue -> Anim BareOrg_interp Name Name;

PersNameTrue -> PersnName_interp (Comma) Anim;
PersNameTrue -> FOA PersnName_interp; 
PersNameTrue -> PersnName_interp (Adv) Verb<gram="sg">;
PersNameTrue -> AnyWord<gram="~S"> Verb<gram="sg"> PersnName_interp;
PersNameTrue -> Word<gnc-agr[1]> Verb<gram="sg", gnc-agr[1]> UCName; //false
PersNameTrue -> Word<kwtype="hyph"> PersnName_interp Comma;
PersNameTrue -> Comma PersnName_interp Comma;
PersNameTrue -> Comma PersnName_interp EOSent;
PersNameTrue -> PersnName_interp Conj PersnName_interp;

BareUCName -> UnknownWord<wff="[А-ЯЁ][а-яё]+", ~l-quoted> ;
BareUCName -> UnknownWord<wff="[А-ЯЁ][а-яё]+-[А-ЯЁ][а-яё]", ~l-quoted> ;
BareUCName2 -> BareUCName<~r-quoted, ~fw> interp (RawName.Name::not_norm; RawName.Name_not_norm::not_norm);
BareUCName2 -> NotAnim UCName interp (RawName.Name; RawName.Name_not_norm::not_norm);// RawName.Misc_check=true); 
BareUCName2 -> NotAnim<gram="nom, sg"> UCName interp (RawName.Name; RawName.Name_not_norm::not_norm;RawName.Misc_check=true); 

Misc -> NotNounNameBad interp (RawName.Misc_check=true);


root -> LocTrue_interp | PersNameTrue  | OrgTrue_interp | BareUCName2 | Misc | SpSurname; 
