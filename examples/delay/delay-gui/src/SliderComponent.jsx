import {useEffect, useState} from "react";
import './App.css';

function SliderComponent({pid, min, max}) {
    const [val, setVal] = useState(0.1);
    const paramCallback = (ev) => {
        const eventPid = ev.detail.paramId;
        if (parseInt(eventPid) !== pid) return;
        const value = parseFloat(ev.detail.value);
        setVal(value);
    }
    useEffect(() => {
        addEventListener("param", paramCallback);
        return function cleanup() {
            removeEventListener("param", paramCallback);
        }
    }, [paramCallback]);

    const sliderDragStarted = (v) => {
        beginParamGesture({
            paramId: pid,
            value: v.target.value
        });
    };
    const sliderChangedCallback = (v) => {
        setVal(v.target.value);
        const args = {
            paramId: pid,
            value: parseFloat(v.target.value)
        };
        setParamValue(args);
    };

    const sliderDragEnded = (v) => {
        endParamGesture({
            paramId: pid,
            value: v.target.value
        });
    };
    return (
        <div>
            <input className="slider-el"
                   type="range"
                   value={val}
                   onDragStart={sliderDragStarted}
                   onChange={sliderChangedCallback}
                   onDragEnd={sliderDragEnded}
                   min={min} max={max} step={0.0001}></input>

        </div>
    )
}

export default SliderComponent;