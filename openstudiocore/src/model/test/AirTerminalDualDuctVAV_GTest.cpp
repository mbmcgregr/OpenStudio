/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include <gtest/gtest.h>
#include <model/test/ModelFixture.hpp>
#include <model/AirTerminalDualDuctVAV.hpp>
#include <model/AirTerminalDualDuctVAV_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/AirLoopHVACZoneSplitter.hpp>
#include <model/AirLoopHVACZoneSplitter_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,AirTerminalDualDuctVAV) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
     Model m; 
     AirTerminalDualDuctVAV terminal(m); 

     exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );

  // Add terminal
  {
    Model m; 
    AirTerminalDualDuctVAV terminal(m); 
    
    AirLoopHVAC airLoopHVAC(m);
    {
      auto t_zoneSplitters = airLoopHVAC.zoneSplitters();
      EXPECT_EQ(1u,t_zoneSplitters.size());
      EXPECT_EQ(airLoopHVAC.zoneSplitter(),t_zoneSplitters.front());
      EXPECT_EQ(1u,airLoopHVAC.demandInletNodes().size());
    }

    EXPECT_TRUE(airLoopHVAC.addBranchForHVACComponent(terminal));
    EXPECT_EQ(2u,airLoopHVAC.demandInletNodes().size());
    EXPECT_EQ(2u,airLoopHVAC.zoneSplitters().size());

    EXPECT_EQ(10u,airLoopHVAC.demandComponents().size());
  }

  {
    // Add terminal with zone
    Model m; 
    AirTerminalDualDuctVAV terminal(m); 
    
    AirLoopHVAC airLoopHVAC(m);
    {
      auto t_zoneSplitters = airLoopHVAC.zoneSplitters();
      EXPECT_EQ(1u,t_zoneSplitters.size());
      EXPECT_EQ(airLoopHVAC.zoneSplitter(),t_zoneSplitters.front());
      EXPECT_EQ(1u,airLoopHVAC.demandInletNodes().size());
    }

    ThermalZone zone(m);
    EXPECT_TRUE(airLoopHVAC.addBranchForZone(zone,terminal));
    {
      auto t_zoneSplitters = airLoopHVAC.zoneSplitters();
      EXPECT_EQ(2u,t_zoneSplitters.size());
      EXPECT_EQ(airLoopHVAC.zoneSplitter(),t_zoneSplitters.front());
      EXPECT_EQ(2u,airLoopHVAC.demandInletNodes().size());
    }

    EXPECT_EQ(12u,airLoopHVAC.demandComponents().size());

    // Remove the terminal
    terminal.remove();
    EXPECT_EQ(7u,airLoopHVAC.demandComponents().size());

    // Make sure we cant add the same zone again
    EXPECT_FALSE(airLoopHVAC.addBranchForZone(zone));

    // Remove the whole branch 
    EXPECT_TRUE(airLoopHVAC.removeBranchForZone(zone));
    EXPECT_EQ(5u,airLoopHVAC.demandComponents().size());

    AirTerminalDualDuctVAV newTerminal(m);
    EXPECT_TRUE(airLoopHVAC.addBranchForZone(zone,newTerminal));
    EXPECT_EQ(12u,airLoopHVAC.demandComponents().size());

    // Remove the whole branch again, this time there is a terminal to worry about
    EXPECT_TRUE(airLoopHVAC.removeBranchForZone(zone));
    EXPECT_EQ(5u,airLoopHVAC.demandComponents().size());
  }

}
