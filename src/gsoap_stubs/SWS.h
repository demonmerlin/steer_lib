//gsoap sws schema namespace: http://www.sve.mvc.mcc.ac.uk/SWS
//gsoap apachesoap schema namespace: http://xml.apache.org/xml-soap
//gsoap soapenc schema namespace: http://schemas.xmlsoap.org/soap/encoding/

//gsoap sws service namespace: http://www.sve.mvc.mcc.ac.uk/SWS

//gsoap sws service location: http://some.where/
//gsoap sws service name: soapSWS
//gsoap wsrp schema namespace: http://www.ibm.com/xmlns/stdwip/web-services/WS-ResourceProperties
//gsoap reg schema namespace: http://www.realitygrid.org/xml/steering
//gsoap wsrl schema namespace: http://www.ibm.com/xmlns/stdwip/web-services/WS-ResourceLifetime

struct sws__GetParamLogResponse {
  xsd__string LogValues;
};

struct sws__RecordCheckpointResponse {
  xsd__string  _RecordCheckpointReturn;
};

struct Command {
  xsd__int Cmd_USCOREid;
  xsd__string Cmd_USCOREname;
};

struct supp_cmds {
  struct Command *__ptr;
  int             __size;
};

struct steerMessage {
  struct supp_cmds Supported_USCOREcommands;
};

struct sws__AttachResponse {
  struct steerMessage ReG_USCOREsteer_USCOREmessage;
};

struct wsrp__DestroyResponse {
  void *_rubbish;
};

struct sws__PutParamLogResponse {
  void *_rubbish;
};

struct GetResourcePropertyRequest {
  xsd__string in_;
};

struct wsrp__GetResourcePropertyResponse {
  xsd__string ResourceProperty;
};

struct wsrp__ResourcePropertyStruct {
  xsd__string ResourceProperty;
};

struct wsrp__GetMultipleResourcePropertiesRequest {
  struct wsrp__ResourcePropertyStruct *__ptr;
  int                                  __size;
};

struct wsrp__SetResourcePropertiesResponse { 
  void *rubbish;
};

struct sws__AddChildRequest {
  xsd__string epr;
  xsd__string name;
};

struct sws__AddChildResponse {
  void *rubbish;
};

struct sws__DetachResponse {
  void *rubbish;
};

//gsoap wsrp service method-action: GetResourceProperty ""
wsrp__GetResourceProperty(//xsd__string _,
			  struct GetResourcePropertyRequest *in_,
			  // char *name,		  
			  XML *out_);
//			   struct wsrp__GetResourcePropertyResponse * out );
//gsoap wsrp service method-action: GetMultipleResourceProperties ""
wsrp__GetMultipleResourceProperties( struct wsrp__GetMultipleResourcePropertiesRequest in,
				     XML *_out);
//gsoap wsrp service method-action: SetResourceProperties ""
wsrp__SetResourceProperties(XML wsrp__Insert,
			    struct wsrp__SetResourcePropertiesResponse *_out);
//gsoap wsrp service method-action: GetResourcePropertyDocument ""
wsrp__GetResourcePropertyDocument(void *_,
				  XML *out_);
//gsoap sws service method-action: AddChild ""
sws__AddChild(struct sws__AddChildRequest __in,
	      struct sws__AddChildResponse *out_);
//gsoap sws service method-action: Attach ""
sws__Attach( void *_, struct sws__AttachResponse *out_);
//sws__Attach( void *_, XML *out_ );
//gsoap sws service method-action: Detach ""
sws__Detach( void *, struct sws__DetachResponse *out );
//gsoap sws service method-action: RecordCheckpoint ""
sws__RecordCheckpoint( xsd__string  in0, xsd__string  in1, 
		       struct sws__RecordCheckpointResponse *out);
//gsoap sws service method-action: GetParamLog ""
sws__GetParamLog( xsd__int  in0, struct sws__GetParamLogResponse *out_);
//gsoap sws service method-action: PutParamLog ""
sws__PutParamLog( xsd__string  in0, struct sws__PutParamLogResponse * out );
//gsoap wsrp service method-action: destroy ""
wsrp__Destroy( void *_, struct wsrp__DestroyResponse * out );
