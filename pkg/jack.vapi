[CCode (cheader_filename = "jack/jack.h")]
namespace Jack
{
    [CCode (cprefix = "", cname="jack_options_t", cheader_filename = "jack/types.h")]
    public enum Options
    {
        JackNullOption = 0x00,
        JackNoStartServer = 0x01,
        JackUseExactName = 0x02,
        JackServerName = 0x04,
        JackLoadName = 0x08,
        JackLoadInit = 0x10,
        JackSessionID = 0x20
    }

    [CCode (cprefix = "", cname="jack_status_t", cheader_filename = "jack/types.h")]
    public enum Status
    {
        JackFailure = 0x01,
        JackInvalidOption = 0x02,
        JackNameNotUnique = 0x04,
        JackServerStarted = 0x08,
        JackServerFailed = 0x10,
        JackServerError = 0x20,
        JackNoSuchClient = 0x40,
        JackLoadFailure = 0x80,
        JackInitFailure = 0x100,
        JackShmFailure = 0x200,
        JackVersionError = 0x400,
        JackBackendError = 0x800,
        JackClientZombie = 0x1000
    }

    [Compact]
    [CCode (cprefix = "jack_", cname = "jack_client_t", free_function = "jack_client_close")]
    public class Client
    {
        [CCode (cname = "jack_client_open")]
        public static Client open(string client_name, Options options, out Status status);
        public void set_xrun_callback(Jack.XRunCallback xrun_callback);
        public void on_shutdown(Jack.ShutdownCallback function);
        public void set_process_callback(Jack.ProcessCallback process_callback);
        public void activate();
        [CCode (array_length = false)]
        public unowned Jack.Port port_register(string port_name, string port_type, ulong flags, ulong buffer_size);
        public unowned Jack.Port port_by_name(string port_name);
        public int connect(string source_port, string destination_port);
    }

    public unowned string port_name(Jack.Port port);

    [Compact]
    [CCode (cname="jack_ringbuffer_t", cheader_filename = "jack/ringbuffer.h")]
    public class Ringbuffer
    {
        [CCode (cname = "jack_ringbuffer_create")]
        public static Ringbuffer create(size_t sz);
    }

    [Compact]
    [CCode (cname="jack_port_t")]
    public class Port<T>
    {
        [CCode (simple_generics = true)]
        public void* get_buffer(Jack.NFrames nframes);
    }

    [Compact]
    [CCode (cname = "jack_default_audio_sample_t")]
    public class DefaultAudioSample
    {

    }

    [SimpleType]
    [IntegerType (rank = 3)]
    [CCode (cname = "jack_nframes_t", has_type_id = false)]
    public struct NFrames {
    }

    [CCode (cprefix = "JackPort", cname = "enum JackPortFlags", cheader_filename = "jack/types.h")]
    public enum JackPort {
        IsInput = 0x1,
        IsOutput = 0x2,
        IsPhysical = 0x4,
        CanMonitor = 0x8,
        IsTerminal = 0x10,
    }
    
    public delegate int XRunCallback();

    public delegate void ShutdownCallback();

    public delegate int ProcessCallback(Jack.NFrames n_frames);

    [CCode (cname = "JACK_DEFAULT_AUDIO_TYPE")]
    const string JACK_DEFAULT_AUDIO_TYPE;

    [CCode (cname = "JACK_DEFAULT_MIDI_TYPE")]
    const string JACK_DEFAULT_MIDI_TYPE;
}