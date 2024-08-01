import React, { useEffect } from 'react';

interface ToastProps {
    message: string;
    onClose: () => void;
    onClick?: () => void; // Optional onClick handler
}

const Toast: React.FC<ToastProps> = ({ message, onClose, onClick }) => {
    useEffect(() => {
        const timer = setTimeout(() => {
            onClose();
        }, 3000); // Toast will disappear after 3 seconds

        return () => clearTimeout(timer);
    }, [onClose]);

    return (
        <div
            className="fixed top-4 right-4 z-50 bg-red-600 text-white p-4 border-2 border-red-800 rounded-lg shadow-lg max-w-xs cursor-pointer"
            onClick={onClick} // Trigger onClick when clicked
        >
            <span>{message}</span>
        </div>
    );
};

export default Toast;
