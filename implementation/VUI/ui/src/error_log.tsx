import React, { useEffect } from 'react';

interface ToastProps {
    message: string;
    onClose: () => void;
}

const Toast: React.FC<ToastProps> = ({ message, onClose }) => {
    useEffect(() => {
        const timer = setTimeout(() => {
            onClose();
        }, 3000); // Toast will disappear after 3 seconds

        return () => clearTimeout(timer);
    }, [onClose]);

    return (
        <div className="fixed bottom-4 right-4 bg-red-600 text-white p-4 rounded shadow-lg">
            <span>{message}</span>
        </div>
    );
};

export default Toast;
