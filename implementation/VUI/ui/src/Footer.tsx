import React from 'react';
import { FaLightbulb, FaRegLightbulb, FaThermometerEmpty } from 'react-icons/fa';
import { RiEmotionNormalFill } from "react-icons/ri";
import { WiHot } from "react-icons/wi";
import { WiMoonAltWaxingCrescent1 } from "react-icons/wi";


const getStatusIcon = (status) => {
  switch (status) {
    case 'on':
      return <RiEmotionNormalFill className="text-yellow-500 text-3xl" />;
    case 'off':
      return <WiHot className="text-red-500 text-3xl" />;
    default:
      return <WiMoonAltWaxingCrescent1 className="text-white-500 text-3xl" />;
  }
};


const Footer = ({ items }) => {
  return (
    <footer className="bg-gray-800 text-white p-4 fixed bottom-0 left-0 w-full">
      <div className="container mx-auto flex flex-row justify-around">
        {items.map((item, index) => (
          <div key={index} className="flex flex-row items-center p-2 border border-gray-600 rounded-lg">
            <div className="flex flex-col items-center mr-4">
              <h2 className="text-lg font-semibold mb-1">{item.name}</h2>
              <p className="text-sm">Temperature: {item.temperature}°C</p>
              <p className={`text-sm ${item.status === 'on' ? 'text-green-500' : 'text-red-500'}`}>
                Status: {item.status.toUpperCase()}
              </p>
            </div>
          </div>
        ))}
        <div className="flex items-center">
          Environment:
          {getStatusIcon("")}
        </div>
      </div>
    </footer>
  );
};


export default Footer;
