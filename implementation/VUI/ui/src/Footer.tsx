import React from "react";
import { RiEmotionNormalFill } from "react-icons/ri";
import { WiHot } from "react-icons/wi";
import { WiMoonAltWaxingCrescent1 } from "react-icons/wi";
import Dashboard from './Dashboard';

// Function to get the appropriate status icon based on the status
const getStatusIcon = (status) => {
  switch (status) {
    case 'on':
      return <RiEmotionNormalFill className="text-yellow-500 text-7xl" />;
    case 'off':
      return <WiHot className="text-red-500 text-7xl" />;
    default:
      return <WiMoonAltWaxingCrescent1 className="text-white text-7xl" />;
  }
};

// Footer component
const Footer = ({ status }) => {
  return (
    <footer className="bg-gray-800 text-white p-4 fixed bottom-0 left-0 w-full">
      <div className="container mx-auto flex flex-row justify-center items-center">
        <Dashboard />
        <div className="flex flex-col items-center text-xl">
          <span>Environment:</span>
          {getStatusIcon("on")}
          <span>{status}</span>
        </div>
      </div>
    </footer>
  );
};

export default Footer;
