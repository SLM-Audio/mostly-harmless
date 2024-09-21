import {useEffect, useState} from "react";

function SliderComponent({pid, name, min, max}) {
    const [val, setVal] = useState(0.0);
    const paramCallback = (ev) => {
        const eventPid = ev.detail.paramId;
        // console.log(ev);
        if(parseInt(eventPid) !== pid) return;
        const value = parseFloat(ev.detail.value);
        setVal(value);
    }
    useEffect(() => {
        addEventListener("param", paramCallback);
        return function cleanup() {
            removeEventListener("param", paramCallback);
        }
    }, []);

    const sliderDragStarted = (v) => {
        beginParamGesture({
            paramId: 0,
            value: v.target.value
        });
    };
    const sliderChangedCallback = (v) => {
        setVal(v.target.value);
        const args = {
            paramId: 0,
            value: parseFloat(v.target.value)
        };
        setParamValue(args).then((res) => console.log(res));
    };

    const sliderDragEnded = (v) => {
        endParamGesture({
            paramId: 0,
            value: v.target.value
        });
    };
    return (
        <>
            <input className="slider-el"
                   type={"range"}
                   value={val}
                   onDragStart={sliderDragStarted}
                   onChange={sliderChangedCallback}
                   onDragEnd={sliderDragEnded}
                   min={0} max={1} step={0.001}></input>

        </>
    )
}

export default SliderComponent;