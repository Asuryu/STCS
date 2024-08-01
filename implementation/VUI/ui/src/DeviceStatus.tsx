import React from "react";
import { FaPowerOff } from "react-icons/fa";

interface DeviceStatusProps {
  isOn: boolean;
  index: number;
}

const DeviceStatus: React.FC<DeviceStatusProps> = ({ isOn, index }) => {
  return (
    <div className={`flex items-center p-4 ${isOn ? 'bg-green-200' : 'bg-red-200'} rounded-lg shadow-lg w-full transition-colors duration-300`}>
      <div className={`flex items-center justify-center w-16 h-16 rounded-full ${isOn ? 'bg-green-500' : 'bg-red-500'} mr-4`}>
        <FaPowerOff className="text-white text-3xl" />
      </div>
      <div>
        <h3 className="text-lg font-bold text-gray-700">Dispositivo {index + 1}</h3>
        <p className="text-3xl font-semibold text-gray-900">{isOn ? 'ON' : 'OFF'}</p>
      </div>
    </div>
  );
};

export default DeviceStatus;
