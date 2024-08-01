import React from "react";
import { RiEmotionNormalFill } from "react-icons/ri";
import { WiHot } from "react-icons/wi";
import { WiMoonAltWaxingCrescent1 } from "react-icons/wi";
import Dashboard from './Dashboard';

interface LastData {
  THERM_01: number;
  THERM_02: number;
  THERM_03: number;
  THERM_04: number;
  HTR_01: string;
  HTR_02: string;
  HTR_03: string;
  HTR_04: string;
  TIMESTAMP: string;
  ENVIRONMENT?: string;
}

const Footer = ({ LastData: LastData}) => {
  const getStatusIcon = (LastData: LastData) => {
    if (LastData) {
      switch (LastData.ENVIRONMENT) {
        case 'Sun Exposure':
          return <WiHot className="text-red-500 text-7xl" />;
        case 'Eclipse':
          return <WiMoonAltWaxingCrescent1 className="text-white text-7xl" />;
        default:
          return <RiEmotionNormalFill className="text-yellow-500 text-7xl" />;
      }
    }
  };

  return (
    <footer className="bg-gray-800 text-white p-4 absolute bottom-0 left-0 w-full mt-20 mx-0">
      <div className="container mx-auto flex flex-row justify-center items-center">
        <Dashboard LastData={LastData} />
        <div className="flex flex-col items-center text-xl">
          <span>Environment:</span>
          {getStatusIcon(LastData)}
        </div>
      </div>
    </footer>
  );
};

export default Footer;
