#define CURRENT_FILE RenderableComponent

#define COMP_INNER(A,B) A ## B
#define COMP(A,B) COMP_INNER(A,B)
#define MY_DUDE COMP(GEN_BODY_,CURRENT_FILE)

MY_DUDE()