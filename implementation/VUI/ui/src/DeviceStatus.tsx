import React from "react";
import { FaPowerOff } from "react-icons/fa";

interface DeviceStatusProps {
  isOn: boolean;
  index: number;
}

const DeviceStatus: React.FC<DeviceStatusProps> = ({ isOn, index }) => {
  return (
    <div className={`flex items-center p-2 ${isOn ? 'bg-green-100' : 'bg-red-100'} rounded-lg shadow-sm max-w-xs transition-colors duration-300`}>
      <div className={`flex items-center justify-center w-12 h-12 rounded-full ${isOn ? 'bg-green-400' : 'bg-red-400'} mr-3`}>
        <FaPowerOff className="text-white text-xl" />
      </div>
      <div>
        <h3 className="text-sm font-semibold text-gray-700">Device {index + 1}</h3>
        <p className="text-ll font-bold text-gray-900">{isOn ? 'ON' : 'OFF'}</p>
      </div>
    </div>
  );
};

export default DeviceStatus;

