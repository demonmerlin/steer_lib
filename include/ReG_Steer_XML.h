/*----------------------------------------------------------------------------
    This header file contains defines routines and data structures for 
    parsing the XML steering-communication messages

    (C)Copyright 2002 The University of Manchester, United Kingdom,
    all rights reserved.

    This software is produced by the Supercomputing, Visualization &
    e-Science Group, Manchester Computing, the Victoria University of
    Manchester as part of the RealityGrid project.

    This software has been tested with care but is not guaranteed for
    any particular purpose. Neither the authors, nor the University of
    Manchester offer any warranties or representations, nor do they
    accept any liabilities with respect to this software.

    This program must not be used for commmercial gain without the
    written permission of the authors.
    
    Supercomputing, Visualization & e-Science Group
    Manchester Computing
    University of Manchester
    Manchester M13 9PL

    email:  csar-advice@cfs.ac.uk.
    Tel:    +44 161 275 6824/5997
    Fax:    +44 161 275 6040    
    
    Date          Version    Updates                            Author
    ----          -------    -------                            ------
    24.9.2002       0.1                                         A Porter

---------------------------------------------------------------------------*/

#include "libxml/xmlmemory.h"
#include "libxml/parser.h"

#define REG_SUCCESS 0
#define REG_FAILURE 1

/*-----------------------------------------------------------------*/

struct param_struct{

  xmlChar             *handle;
  xmlChar             *label;
  xmlChar             *value;
  xmlChar             *type;
  xmlChar             *steerable;
  xmlChar             *is_internal;
  struct param_struct *next;
};

struct cmd_struct{

  xmlChar             *id;
  struct param_struct *first_param;
  struct param_struct *param;
  struct cmd_struct   *next;
};

struct status_struct{

  struct param_struct *first_param;
  struct param_struct *param;
  struct cmd_struct   *first_cmd;
  struct cmd_struct   *cmd;
};

struct io_struct{

  xmlChar  	   *label;
  xmlChar  	   *handle;
  xmlChar  	   *direction;
  xmlChar  	   *support_auto;
  xmlChar  	   *freq_handle;
  struct io_struct *next;
};

struct control_struct{

  struct param_struct *first_param;
  struct param_struct *param;
  struct cmd_struct   *first_cmd;
  struct cmd_struct   *cmd;
};

struct supp_cmd_struct{

  struct cmd_struct   *first_cmd;
  struct cmd_struct   *cmd;
};

struct io_def_struct{
  
  struct io_struct *first_io;
  struct io_struct *io;
};

static struct msg_struct{

  int   	           msg_type;  
  struct status_struct    *status;
  struct control_struct   *control;
  struct supp_cmd_struct  *supp_cmd;
  struct io_def_struct    *io_def;
} msg;

/*-----------------------------------------------------------------*/

int Parse_xml(xmlDocPtr doc, struct msg_struct *msg);
int Parse_xml_file(char* filename, struct msg_struct *msg);
int Parse_xml_buf(char* buf, int size, struct msg_struct *msg);

int parseStatus(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,
	        struct status_struct *status);
int parseControl(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,
	         struct control_struct *ctrl);
int parseSuppCmd(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,
		 struct supp_cmd_struct *supp_cmd);
int parseParam(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,
	       struct param_struct *param);
int parseCmd(xmlDocPtr doc, xmlNsPtr ns, xmlNodePtr cur,
	     struct cmd_struct *cmd);

struct msg_struct       *New_msg_struct();
struct status_struct    *New_status_struct();
struct control_struct   *New_control_struct();
struct supp_cmd_struct  *New_supp_cmd_struct();
struct io_def_struct    *New_io_def_struct();
struct io_struct        *New_io_struct();
struct param_struct     *New_param_struct();
struct cmd_struct       *New_cmd_struct();

void Delete_msg_struct(struct msg_struct *msg);
void Delete_status_struct(struct status_struct *status);
void Delete_control_struct(struct control_struct *ctrl);
void Delete_supp_cmd_struct(struct supp_cmd_struct *supp_cmd);
void Delete_param_struct(struct param_struct *param);
void Delete_cmd_struct(struct cmd_struct *cmd);
void Delete_io_def_struct(struct io_def_struct *io_def);
void Delete_io_struct(struct io_struct *io);

void Print_msg(struct msg_struct *msg);
void Print_status_struct(struct status_struct *status);
void Print_control_struct(struct control_struct *ctrl);
void Print_param_struct(struct param_struct *param);
void Print_cmd_struct(struct cmd_struct *cmd);
void Print_supp_cmd_struct(struct supp_cmd_struct *supp_cmd);
void Print_io_def_struct(struct io_def_struct   *io_def);
void Print_io_struct(struct io_struct   *io);

