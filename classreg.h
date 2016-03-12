//usr/bin/chmod -x "$0"; exit
/* classreg.h
 * 
 * Copyright (C) 2016 by Henry Kroll III, www.thenerdshow.com 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */
#define DEBUG 0
#ifndef CLASSREG
#define CLASSREG
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
//~ class prototypes
void ClInstance (int methods,...);
//~ exposed public methods
enum classMethods {_CREATE, _DESTROY, _INFO, _APPEND, _INSERT};
#define IMPLEMENTS(a,...) enum {a=__LINE__,__VA_ARGS__}
#define _STR(a) #a
#define STR(a) _STR(a)
//~ rudimentary error handling
#define TRYTO(a) if (!(a)) { \
    fprintf(stderr,"%s:%i: ",__FILE__,__LINE__); \
    perror(#a); \
    exit (1); }
//~ Model Facades for Proxying in-line functions
#define forEach(s, t) for (int index=0;(t = s[index]);index++)
#define split(b,c,ci) for(ci=strtok(b,c);ci;(ci=strtok(NULL,c)))
#define FREE(a) if(a){free(a);a=NULL; \
    INFO(#a" freed.\n"); \
    }
#define INFO(...) if (DEBUG) \
    fprintf(stderr,"info: " __VA_ARGS__)
#define ERR(...) {\
    fprintf(stderr,"err: " __VA_ARGS__); \
    exit(1);}

#define endl puts("");
// a random integer within specified range (MSDN)
#define getrandom(min, max) \
    ((rand()%(int)(((max) + 1)-(min)))+ (min))
//~ subclass the function object to make class objects
#define class(a,...) a(int methods, __VA_ARGS__)
#define use_classes void *_classes_used=NULL;
#define public va_list ap; va_start(ap,methods);switch
#define method case
#define param(a,b) b a=va_arg(ap,b);
#define end_class va_end(ap);

//~ subclass the return object for automatic cleanup!
#define return if(_classes_used) { \
    ClInstance(_DESTROY, &_classes_used); } return

//~ Abstract insertion and destruction Method prototypes
#define new_array(char,newarray,a,b) \
    char **newarray=new_##char##_array(&_classes_used,a,b)
#define destroy_array(char,newarray,a) \
    destroy_##char##_array(newarray, 5);
//~ Factory Method for reusable class templates
#define new_array_factory(char)                                        \
    void destroy_##char##_array (char **array, int d1) {               \
        if (array) {                                                   \
            for (int i=0;i<d1;i++) FREE (array[i]);                    \
            FREE (array);                                              \
    }   }                                                              \
    char **new_##char##_array (void **_classes, int d1, int d2) {      \
        char **newarray=malloc ((d1+1)*sizeof(char**));                \
        ClInstance(_CREATE, _classes, newarray,                        \
            destroy_##char##_array, d1);                               \
        for (int i=0;i<d1;i++) {                                       \
            newarray[i]=malloc (d2*sizeof(char*));                     \
        } newarray[d1]=NULL;return newarray;                           \
    }
#endif
