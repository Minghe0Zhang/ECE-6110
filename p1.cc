#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/netanim-module.h"  
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-layout-module.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("P1");
int main(int argc, char *argv[]){

  SeedManager::SetSeed(1);

  Time::SetResolution (Time::NS);

	uint32_t nSpokes = 8;
  uint32_t endTime = 60.0;
  std::string transport_prot = "TcpWestwood";
  //Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (250));
  //Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("5kb/s"));

	CommandLine cmd;
	cmd.AddValue ("nSpokes", "Number of nodes to place in the star", nSpokes);
  cmd.AddValue ("transport_prot", "Transport protocol to use: TcpNewReno, "
                "TcpHybla, TcpHighSpeed, TcpHtcp, TcpVegas, TcpScalable, TcpVeno, "
                "TcpBic, TcpYeah, TcpIllinois, TcpWestwood, TcpWestwoodPlus ", transport_prot);
	cmd.Parse (argc, argv);
  //std::cout<<nSpokes<<std::endl;

  // 4 MB of TCP buffer 
  //Config::SetDefault ("ns3::TcpSocket::RcvBufSize", UintegerValue (1 << 21));
  //Config::SetDefault ("ns3::TcpSocket::SndBufSize", UintegerValue (1 << 21));
    // Select TCP variant
  if (transport_prot.compare ("TcpNewReno") == 0)
    {
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpNewReno::GetTypeId ()));
    }
  else if (transport_prot.compare ("TcpHybla") == 0)
    {
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpHybla::GetTypeId ()));
    }
  else if (transport_prot.compare ("TcpHighSpeed") == 0)
    {
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpHighSpeed::GetTypeId ()));
    }
  else if (transport_prot.compare ("TcpVegas") == 0)
    {
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpVegas::GetTypeId ()));
    }
  else if (transport_prot.compare ("TcpScalable") == 0)
    {
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpScalable::GetTypeId ()));
    }
  else if (transport_prot.compare ("TcpHtcp") == 0)
    {
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpHtcp::GetTypeId ()));
    }
  else if (transport_prot.compare ("TcpVeno") == 0)
    {
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpVeno::GetTypeId ()));
    }
  else if (transport_prot.compare ("TcpBic") == 0)
    {
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpBic::GetTypeId ()));
    }
  else if (transport_prot.compare ("TcpYeah") == 0)
    {
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpYeah::GetTypeId ()));
    }
  else if (transport_prot.compare ("TcpIllinois") == 0)
    {
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpIllinois::GetTypeId ()));
    }
  else if (transport_prot.compare ("TcpWestwood") == 0)
    { // the default protocol type in ns3::TcpWestwood is WESTWOOD
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpWestwood::GetTypeId ()));
      Config::SetDefault ("ns3::TcpWestwood::FilterType", EnumValue (TcpWestwood::TUSTIN));
    }
  else if (transport_prot.compare ("TcpWestwoodPlus") == 0)
    {
      Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpWestwood::GetTypeId ()));
      Config::SetDefault ("ns3::TcpWestwood::ProtocolType", EnumValue (TcpWestwood::WESTWOODPLUS));
      Config::SetDefault ("ns3::TcpWestwood::FilterType", EnumValue (TcpWestwood::TUSTIN));
    }
  else
    {
      NS_LOG_DEBUG ("Invalid TCP version");
      exit (1);
    }

  //std::cout<<"TCP variant is: "<<transport_prot<<std::endl;


	//set hub - spokes links
  PointToPointHelper spoke_link1;
  spoke_link1.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  spoke_link1.SetChannelAttribute ("Delay", StringValue ("10ms"));
  PointToPointStarHelper star1(nSpokes, spoke_link1);

  NS_LOG_INFO ("Install internet stack on all nodes.");
  InternetStackHelper internet1;
  star1.InstallStack (internet1);
    
  NS_LOG_INFO ("Assign IP Addresses.");
  star1.AssignIpv4Addresses (Ipv4AddressHelper ("10.1.1.0", "255.255.255.0"));
    
  //NetDeviceContainer spoke_devices1;
  //spoke_devices1 = spoke_link1.Install(hubs.Get(0),);

  PointToPointHelper spoke_link2;
  spoke_link2.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  spoke_link2.SetChannelAttribute ("Delay", StringValue ("10ms"));
  PointToPointStarHelper star2(nSpokes, spoke_link2);
  //star2.m_hub = hubs.Get(1);
  InternetStackHelper internet2;
  star2.InstallStack(internet2);
  star2.AssignIpv4Addresses (Ipv4AddressHelper ("10.2.1.0", "255.255.255.0"));

  //std::cout<<"topology spokes test"<<std::endl;



  //create hubs
  NS_LOG_INFO ("Create hubs");
  NodeContainer hubs;
  hubs.Add(star1.GetHub());
  hubs.Add(star2.GetHub());

  //set hub - hub link
  NS_LOG_INFO ("Create hub_links.");
  PointToPointHelper hub_link;
  hub_link.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  hub_link.SetChannelAttribute ("Delay", StringValue ("20ms"));

  NetDeviceContainer hub_devices; 
  hub_devices = hub_link.Install (hubs);

  Ipv4AddressHelper address;
  Ipv4InterfaceContainer interfaces = address.Assign (hub_devices);


  //uint16_t port = 5000;  // well-known echo port number



//
// Create a PacketSinkApplication and install it on node 1
//
  //PacketSinkHelper sink ("ns3::TcpSocketFactory",
    //                     InetSocketAddress (Ipv4Address::GetAny (), port));
  /*
  ApplicationContainer sinkApps;
  for (uint32_t i = 0; i < nSpokes; ++i)
  {
    sinkApps.Add(sink.Install (star2.GetSpokeNode (i)));
  }
  sinkApps.Start (Seconds (1.0));*/
  //sinkApps.Stop (Seconds (endTime));
  Address sinkLocalAddress(InetSocketAddress(Ipv4Address::GetAny(), 5000));
  PacketSinkHelper sinkHelper ("ns3::TcpSocketFactory", sinkLocalAddress);
  ApplicationContainer sinkApps;
  for(uint32_t i = 0; i < nSpokes; ++i) {
    sinkApps.Add(sinkHelper.Install(star2.GetSpokeNode(i)));
  }

  sinkApps.Start(Seconds(1.0));

  BulkSendHelper bulkSender ("ns3::TcpSocketFactory", Address());
  bulkSender.SetAttribute("SendSize", UintegerValue(1024));
  ApplicationContainer sourceApps;
  for (uint32_t i = 0; i < nSpokes; ++i)
  {
    /*

    BulkSendHelper source ("ns3::TcpSocketFactory", InetSocketAddress(star2.GetSpokeIpv4Address(i), port));
    source.SetAttribute ("MaxBytes", UintegerValue (0));
    sourceApps.Add(source.Install (star1.GetSpokeNode(i)));*/
    //int32_t remoteNode = (i + 1) % nSpokes;
    //AddressValue remoteAddress(InetSocketAddress(star2.GetSpokeIpv4Address(remoteNode), 5000));
    AddressValue remoteAddress(InetSocketAddress(star2.GetSpokeIpv4Address(i), 5000));
    bulkSender.SetAttribute("Remote", remoteAddress);
    sourceApps.Add(bulkSender.Install(star1.GetSpokeNode(i)));
  }
  
  sourceApps.Start (Seconds (2.0));
  //sourceApps.Stop (Seconds (endTime));




  //std::cout<<"output some info about sinkApps!"<<std::endl;
  //cout info
  
  
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  Simulator::Stop(Seconds(endTime));
	Simulator::Run ();
  uint64_t totalRx = 0;

  for(uint32_t i = 0; i < nSpokes; ++i) {

    Ptr<PacketSink> sink = DynamicCast<PacketSink>(sinkApps.Get(i));
    uint32_t bytesReceived = sink->GetTotalRx();
    totalRx += bytesReceived;
    std::cout << "Sink " << i << "\tTotalRx: " << bytesReceived * 1e-6 * 8 << "Mb";
    std::cout << "\tThroughput: " << (bytesReceived * 1e-6 * 8) / endTime << "Mbps" << std::endl;
  }

  std::cout << std::endl;
  std::cout << "Totals\tTotalRx: " << totalRx * 1e-6 * 8 << "Mb";
  std::cout << "\tThroughput: " << (totalRx * 1e-6 * 8) / endTime << "Mbps" << std::endl;

  Simulator::Destroy ();

  return 0;
}
