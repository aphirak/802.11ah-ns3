/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright (c) 2007,2008,2009 INRIA, UDCAST
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Amine Ismail <amine.ismail@sophia.inria.fr>
 *                      <amine.ismail@udcast.com>
 */

#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "packet-loss-counter.h"

#include "seq-ts-header.h"

#include "ns3/ipv4.h"
#include "ns3/ipv6.h"
#include "coap-server.h"
#include <climits>

#include "arpa/inet.h"
#include "coap-pdu.h"
#include "seq-ts-header.h"

#define WITH_SEQ_TS true

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("CoapServer");

NS_OBJECT_ENSURE_REGISTERED(CoapServer);

TypeId CoapServer::GetTypeId(void) {
	static TypeId tid =
			TypeId("ns3::CoapServer")
			.SetParent<Application>()
			.SetGroupName("Applications")
			.AddConstructor<CoapServer>()
			.AddAttribute("Port", "Port on which we listen for incoming packets.",
					UintegerValue(100),
					MakeUintegerAccessor(&CoapServer::m_port),
					MakeUintegerChecker<uint16_t>())
			.AddTraceSource("Rx",
					"A packet is received",
					MakeTraceSourceAccessor(&CoapServer::m_packetReceived),
					"ns3::CoapServer::PacketReceivedCallback")
					;

	return tid;
}

CoapServer::CoapServer() : m_lossCounter(0) {
	NS_LOG_FUNCTION(this);
	m_received = 0;
	m_sent = 0;
	m_coapCtx = NULL;
	m_sendEvent = EventId ();
	m_fromSocket = 0;
}

CoapServer::~CoapServer() {
	NS_LOG_FUNCTION(this);
	for(coap_resource_t* resource : m_resourceVectorPtr){
		coap_delete_resource(m_coapCtx, resource->key);
	}
	if (m_coapCtx)
		coap_free_context(m_coapCtx);
	m_socket = 0;
	m_socket6 = 0;
}

uint16_t CoapServer::GetPacketWindowSize() const {
	NS_LOG_FUNCTION(this);
	return m_lossCounter.GetBitMapSize();
}

void CoapServer::SetPacketWindowSize(uint16_t size) {
	NS_LOG_FUNCTION(this << size);
	m_lossCounter.SetBitMapSize(size);
}

uint32_t CoapServer::GetLost(void) const {
	NS_LOG_FUNCTION(this);
	return m_lossCounter.GetLost();
}

uint32_t CoapServer::GetReceived(void) const {
	NS_LOG_FUNCTION(this);
	return m_received;
}

bool CoapServer::PrepareContext(void)
{
	  coap_address_t srcAddr;
	  coap_address_init(&srcAddr);

	  if (true)
	  {
		  srcAddr.addr.sin.sin_family      = AF_INET;
		  srcAddr.addr.sin.sin_port        = htons(COAP_DEFAULT_PORT);
		  srcAddr.addr.sin.sin_addr.s_addr = INADDR_ANY;
		  m_coapCtx = coap_new_context(&srcAddr);
	  }
	  else
	  {
		  srcAddr.addr.sin.sin_family      = AF_INET6;
		  srcAddr.addr.sin.sin_port        = htons(COAP_DEFAULT_PORT);
		  srcAddr.addr.sin.sin_addr.s_addr = inet_addr("::");
		  m_coapCtx = coap_new_context(&srcAddr);
	  }
	  m_coapCtx->network_send = CoapServer::coap_network_send;
      m_coapCtx->ns3_coap_server_obj_ptr = this;

	  if (m_coapCtx) return true;
	  else return false;
}

#define INDEX "This is a test server made with libcoap (see https://libcoap.net)\n" \
              "Copyright (C) 2010--2016 Olaf Bergmann <bergmann@tzi.org>\n\n"

void CoapServer::HndGetIndex(coap_context_t *ctx UNUSED_PARAM,
        struct coap_resource_t *resource UNUSED_PARAM,
        const coap_endpoint_t *local_interface UNUSED_PARAM,
        coap_address_t *peer UNUSED_PARAM,
        coap_pdu_t *request UNUSED_PARAM,
        str *token UNUSED_PARAM,
        coap_pdu_t *response) {
  uint8_t buf[3];

  response->hdr->code = COAP_RESPONSE_CODE(205);

  coap_add_option(response,
                  COAP_OPTION_CONTENT_TYPE,
                  coap_encode_var_bytes(buf, COAP_MEDIATYPE_TEXT_PLAIN), buf);

  coap_add_option(response,
                  COAP_OPTION_MAXAGE,
                  coap_encode_var_bytes(buf, 0x2ffff), buf);

  coap_add_data(response, strlen(INDEX), (unsigned char *)INDEX);
}

void CoapServer::HndHello(coap_context_t *ctx UNUSED_PARAM,
		struct coap_resource_t *resource UNUSED_PARAM,
		const coap_endpoint_t *local_interface UNUSED_PARAM,
		coap_address_t *peer UNUSED_PARAM,
		coap_pdu_t *request UNUSED_PARAM,
		str *token UNUSED_PARAM,
		coap_pdu_t *response) {
	uint8_t buf[3];
	response->hdr->code = COAP_RESPONSE_CODE(205);
	coap_add_option(response,
			COAP_OPTION_CONTENT_TYPE,
			coap_encode_var_bytes(buf, COAP_MEDIATYPE_TEXT_PLAIN), buf);
	const char* responseData ("Hello World!");
	coap_add_data(response, strlen(responseData), (unsigned char *)responseData);
}

void CoapServer::HndGetControlValue(coap_context_t *ctx,
        struct coap_resource_t *resource,
        const coap_endpoint_t *local_interface UNUSED_PARAM,
        coap_address_t *peer UNUSED_PARAM,
        coap_pdu_t *request ,
        str *token UNUSED_PARAM,
		coap_pdu_t *response)
{
	uint8_t buf[4];
	// If m_controlValue was deleted, we pretend to have no such resource
	response->hdr->code = (CoapServer::m_controlValue == 0) ? COAP_RESPONSE_CODE(205) : COAP_RESPONSE_CODE(404);
	coap_add_option(response, COAP_OPTION_CONTENT_TYPE, coap_encode_var_bytes(buf, COAP_MEDIATYPE_TEXT_PLAIN), buf);
	if (response->hdr->code != COAP_RESPONSE_CODE(404))
	{
		const char* responseData (std::to_string(CoapServer::m_controlValue).c_str());
		coap_add_data(response, strlen(responseData), (unsigned char *)responseData);
	}
}

double CoapServer::m_controlValue = 0;
double CoapServer::m_sensorValue = 0;

void CoapServer::HndPutControlValue(coap_context_t *ctx UNUSED_PARAM,
		struct coap_resource_t *resource UNUSED_PARAM,
		const coap_endpoint_t *local_interface UNUSED_PARAM,
		coap_address_t *peer UNUSED_PARAM,
		coap_pdu_t *request,
		str *token UNUSED_PARAM,
		coap_pdu_t *response)
{
	size_t size;
	unsigned char *data;

	// If there is no control value, we pretend that such a resource does not exist
	// Response code: if created 201, if exists 204, if not allowed 405
	response->hdr->code = (CoapServer::m_controlValue != 0) ? COAP_RESPONSE_CODE(204) : COAP_RESPONSE_CODE(201);
	resource->dirty = 1;

	// coap_get_data() sets size to 0 on error
	(void)coap_get_data(request, &size, &data);

	if (size == 0)
		NS_LOG_INFO("Empty PUT request.");
	else {
		std::stringstream dataStringStream;
		for (uint32_t i = 0; i < size; i++)
		{
			dataStringStream << data[i];
		}
		CoapServer::m_controlValue = std::stod(dataStringStream.str());
		NS_LOG_INFO("Successfull PUT request. New control value is " << CoapServer::m_controlValue);
	}

	// Reply with a sensor reading
	uint8_t buf[4];
	coap_add_option(response, COAP_OPTION_CONTENT_TYPE, coap_encode_var_bytes(buf, COAP_MEDIATYPE_TEXT_PLAIN), buf);
	CoapServer::m_sensorValue += 0.001;
	const char* responseData (std::to_string(CoapServer::m_sensorValue).c_str());
	coap_add_data(response, strlen(responseData), (unsigned char *)responseData);
}

void CoapServer::HndDeleteControlValue(coap_context_t *ctx UNUSED_PARAM,
        struct coap_resource_t *resource UNUSED_PARAM,
        const coap_endpoint_t *local_interface UNUSED_PARAM,
        coap_address_t *peer UNUSED_PARAM,
        coap_pdu_t *request UNUSED_PARAM,
        str *token UNUSED_PARAM,
        coap_pdu_t *response UNUSED_PARAM)
{
	CoapServer::m_controlValue = 0;
}

void CoapServer::InitializeResources(coap_context_t* ctx, std::vector<coap_resource_t*> resources){
	coap_resource_t *r;

	// empty resource handler responds with INDEX
	r = coap_resource_init(NULL, 0, 0);
	coap_register_handler(r, COAP_REQUEST_GET, HndGetIndex);

	coap_add_attr(r, (unsigned char *)"ct", 2, (unsigned char *)"0", 1, 0);
	coap_add_attr(r, (unsigned char *)"title", 5, (unsigned char *)"\"General Info\"", 14, 0);
	coap_add_resource(ctx, r);
	resources.push_back(r);

	// control resource for control loop simulation
	r = coap_resource_init((uint8_t *)"control", 7, COAP_RESOURCE_FLAGS_NOTIFY_CON);
	coap_register_handler(r, COAP_REQUEST_GET, HndGetControlValue);
	coap_register_handler(r, COAP_REQUEST_PUT, HndPutControlValue);
	coap_register_handler(r, COAP_REQUEST_DELETE, HndDeleteControlValue);
	coap_add_attr(r, (uint8_t *)"ct", 2, (uint8_t *)"0", 1, 0);
	coap_add_attr(r, (uint8_t *)"title", 5, (uint8_t *)"\"Control Value\"", 16, 0);
	coap_add_resource(ctx, r);
	resources.push_back(r);

	// dummy hello resource responds with plain text "Hello World!"
	r = coap_resource_init((uint8_t *)"hello", 5, COAP_RESOURCE_FLAGS_NOTIFY_NON);
	coap_register_handler(r, COAP_REQUEST_GET, HndHello);
	coap_add_resource(ctx, r);
	resources.push_back(r);
}

void CoapServer::DoDispose(void) {
	NS_LOG_FUNCTION(this);
	Application::DoDispose();
}

void CoapServer::StartApplication(void) {
	NS_LOG_FUNCTION(this);

	// Prepare context for IPv4 and IPv6
	if (!m_coapCtx){
		if (PrepareContext())
			NS_LOG_INFO("CoapServer::Coap context ready.");
		else{
			NS_LOG_WARN("CoapServer::No context available for the interface. Abort.");
			return;
		}
	}

	// Initialize resources
	CoapServer::InitializeResources(m_coapCtx, m_resourceVectorPtr);
	if (m_socket == 0) {
		TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
		m_socket = Socket::CreateSocket(GetNode(), tid);
		InetSocketAddress local = InetSocketAddress(Ipv4Address::GetAny(), 	m_port);
		m_socket->Bind(local);
	}
	if (m_socket6 == 0) {
		TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
		m_socket6 = Socket::CreateSocket(GetNode(), tid);
		Inet6SocketAddress local = Inet6SocketAddress(Ipv6Address::GetAny(), m_port);
		m_socket6->Bind(local);
	}
	m_socket->SetRecvCallback (MakeCallback (&CoapServer::HandleRead, this));
	m_socket6->SetRecvCallback (MakeCallback (&CoapServer::HandleRead, this));
}

void CoapServer::StopApplication() {
	NS_LOG_FUNCTION(this);

	if (m_socket != 0) {
		m_socket->Close ();
		m_socket->SetRecvCallback(MakeNullCallback<void, Ptr<Socket> >());
	}
	if (m_socket6 != 0) {
		m_socket6->Close ();
		m_socket6->SetRecvCallback(MakeNullCallback<void, Ptr<Socket> >());
	}
	Simulator::Cancel (m_sendEvent);
}

void
CoapServer::coap_transaction_id(const coap_pdu_t *pdu, coap_tid_t *id) {
	coap_key_t h;
	memset(h, 0, sizeof(coap_key_t));

	// Compare the transport address.
	if (InetSocketAddress::IsMatchingType(m_from)) {
		std::stringstream addressStringStream;
		addressStringStream << InetSocketAddress::ConvertFrom (m_from).GetPort();
		std::string port (addressStringStream.str());
		coap_hash((const unsigned char *)port.c_str(), sizeof(port.c_str()), h);
		addressStringStream.str("");
		addressStringStream << InetSocketAddress::ConvertFrom (m_from).GetIpv4 ();
		std::string ipv4 (addressStringStream.str());
		coap_hash((const unsigned char *)ipv4.c_str(), sizeof(ipv4.c_str()), h);
	}
	else if (Inet6SocketAddress::IsMatchingType(m_from)) {
		std::stringstream addressStringStream;
		addressStringStream << Inet6SocketAddress::ConvertFrom (m_from).GetPort();
		std::string port (addressStringStream.str());
		//std::cout << "port is " << port << std::endl;
		coap_hash((const unsigned char *)port.c_str(), sizeof(port.c_str()), h);
		addressStringStream.str("");
		addressStringStream << Inet6SocketAddress::ConvertFrom (m_from).GetIpv6();
		std::string ipv6 (addressStringStream.str());
		//std::cout << "ipv6 is " << ipv6 << std::endl;
		coap_hash((const unsigned char *)ipv6.c_str(), sizeof(ipv6.c_str()), h);
	}
	else return;

	coap_hash((const unsigned char *)&pdu->hdr->id, sizeof(unsigned short), h);
	*id = (((h[0] << 8) | h[1]) ^ ((h[2] << 8) | h[3])) & INT_MAX;
}

ssize_t CoapServer::CoapHandleMessage(Ptr<Packet> packet){
	uint32_t msg_len (packet->GetSize());
	ssize_t bytesRead(0);
	uint8_t* msg = new uint8_t[msg_len];
	bytesRead = packet->CopyData (msg, msg_len);

	if (msg_len < sizeof(coap_hdr_t)) {
		NS_LOG_ERROR(this << "Discarded invalid frame CoAP" );
		return 0;
	}

	// check version identifier
	if (((*msg >> 6) & 0x03) != COAP_DEFAULT_VERSION) {
		NS_LOG_ERROR(this << "Unknown CoAP protocol version " << ((*msg >> 6) & 0x03));
		return 0;
	}

	coap_queue_t * node = coap_new_node();
	if (!node) {
		return 0;
	}

	node->pdu = coap_pdu_init(0, 0, 0, msg_len);
	if (!node->pdu) {
		coap_delete_node(node);
		return 0;
	}

	NS_ASSERT(node->pdu);
	if (coap_pdu_parse(msg, msg_len, node->pdu))
	{
		coap_show_pdu(node->pdu);
	}
	else {
		NS_LOG_ERROR("Coap server cannot parse CoAP packet. Abort.");
		return 0;
	}
	this->coap_transaction_id(node->pdu, &node->id);
	coap_dispatch(m_coapCtx, node);
	return bytesRead;
}

ssize_t
CoapServer::coap_network_send(struct coap_context_t *context UNUSED_PARAM,
		  const coap_endpoint_t *local_interface,
		  const coap_address_t *dst,
		  unsigned char *data,
		  size_t datalen)
{
	unused(local_interface);
	unused(dst);
	unused(data);
	unused(datalen);
	CoapServer* serverPtr = static_cast<CoapServer*>(context->ns3_coap_server_obj_ptr);

	return (*serverPtr).Send(data, datalen);
}

ssize_t
CoapServer::Send (uint8_t *data, size_t datalen)
{
	NS_LOG_FUNCTION (this);
	NS_ASSERT (m_sendEvent.IsExpired ());

	Ptr<Packet> p = Create<Packet> (data, datalen);

	ssize_t bytesSent;
    p->RemoveAllPacketTags ();
    p->RemoveAllByteTags ();

	NS_ASSERT(m_fromSocket);

	// add sequence header to the packet
#ifdef WITH_SEQ_TS
	SeqTsHeader seqTs;
	seqTs.SetSeq (m_sent);
	p->AddHeader (seqTs);
#endif

	if ((m_fromSocket->SendTo (p, 0, m_from)) >= 0)
	{
		++m_sent;
		bytesSent = p->GetSize();
		if (InetSocketAddress::IsMatchingType (m_from))
		{
			NS_LOG_INFO ("At time " << Simulator::Now ().GetSeconds () << "s server sent " << bytesSent << " bytes to " <<
					InetSocketAddress::ConvertFrom (m_from).GetIpv4 () << " port " <<
					InetSocketAddress::ConvertFrom (m_from).GetPort () << " Uid: " << p->GetUid());
		}
		else if (Inet6SocketAddress::IsMatchingType (m_from))
		{
			NS_LOG_INFO ("At time " << Simulator::Now ().GetSeconds () << "s server sent " << bytesSent << " bytes to " <<
					Inet6SocketAddress::ConvertFrom (m_from).GetIpv6 () << " port " <<
					Inet6SocketAddress::ConvertFrom (m_from).GetPort () << " Uid: " << p->GetUid());
		}

	}
	else
	{
		std::stringstream peerAddressStringStream;
		if (Ipv4Address::IsMatchingType (m_from))
			peerAddressStringStream << Ipv4Address::ConvertFrom (m_from);
		else if (Ipv6Address::IsMatchingType (m_from))
			peerAddressStringStream << Ipv6Address::ConvertFrom (m_from);
		NS_LOG_INFO ("Error while sending " << p->GetSize() << " bytes to " << peerAddressStringStream.str ());
		bytesSent = -1;
	}
	return bytesSent;
}

// Read from ns3 socket
void CoapServer::HandleRead(Ptr<Socket> socket) {
	NS_LOG_FUNCTION(this << socket);
	m_fromSocket = socket;
	Ptr<Packet> packet;
	while ((packet = socket->RecvFrom(m_from))) {
		if (packet->GetSize() > 0) {
			m_packetReceived(packet, m_from);
		}
		if (InetSocketAddress::IsMatchingType(m_from)) {
			NS_LOG_INFO("Server Received " << packet->GetSize () << " bytes from " << InetSocketAddress::ConvertFrom (m_from).GetIpv4 () << " at " << Simulator::Now ().GetSeconds() << "s" << " Uid: " << packet->GetUid ());
		} else if (Inet6SocketAddress::IsMatchingType(m_from)) {
			NS_LOG_INFO("TraceDelay: RX " << packet->GetSize () << " bytes from "<< Inet6SocketAddress::ConvertFrom (m_from).GetIpv6 ()  << " Uid: " << packet->GetUid () << " RXtime: " << Simulator::Now ());
		}
#ifdef WITH_SEQ_TS
		SeqTsHeader seqTs;
		packet->RemoveHeader(seqTs);
		//uint32_t currentSequenceNumber = seqTs.GetSeq();
#endif
		packet->RemoveAllPacketTags ();
		packet->RemoveAllByteTags ();

		//m_lossCounter.NotifyReceived(currentSequenceNumber);
		m_received++;

		if (!this->CoapHandleMessage(packet)){
			NS_LOG_ERROR("Cannot handle message. Abort.");
			return;
		}
	}
}

} // Namespace ns3
